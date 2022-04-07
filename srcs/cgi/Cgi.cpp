/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:54 by pohl              #+#    #+#             */
/*   Updated: 2022/04/07 10:49:18 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responses/httpExceptions.hpp"
#include "cgi/Cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <errno.h>
#include <exception>

Cgi::Cgi( Rules& rules, iRequest* request ):
	_rules(rules), _request(request), _envp(NULL), _argv(NULL)
{
	return;
}

Cgi::~Cgi( void )
{
	freeArgv();
	freeEnvp();
	return;
}

void	Cgi::executeCgi( std::string requestedFilePath, std::vector<unsigned char>& body )
{
	int			forkPid;
	int			returnValue;

	createPipe(_pipeFd);
	forkPid = createFork();
	if (isChildProcess(forkPid))
	{
		executeChildProcess(requestedFilePath, body);
		exit(0);
	}
	waitpid(forkPid, &returnValue, 0);
	returnValue = WEXITSTATUS(returnValue);
	if (returnValue == EACCES)
		throw httpError(403);
	if (returnValue == ENOENT)
		throw httpError(404);
	if (returnValue != 0)
		throw httpError(500);
	readCgiOutput();
}

int	Cgi::parseAndRemoveHeaders( response& response )
{
	std::string rawHeaders = popHeadersFromCgiOutput();

	response.replaceFieldToHeaderMap(std::make_pair("Content-Type", "text/plain"));
	return writeHeadersToResponse(rawHeaders, response);
}

std::string Cgi::writeBodyToTmpFile( void )
{
	const char*	fileName = "/tmp/webserv-tmp-cgi";
	int			fileFd;

	remove(fileName);
	fileFd = open(fileName, O_WRONLY | O_CREAT,
			S_IRUSR | S_IRGRP | S_IROTH);
	if (fileFd == -1)
		throw serverError("Could not open cgi output file for writing");
	if (write(fileFd, _rawCgiOutput.c_str(), _rawCgiOutput.size())
			!= static_cast<ssize_t>(_rawCgiOutput.size()))
		throw serverError("Problem writing to cgi output file");
	close(fileFd);
	return fileName;
}

void	Cgi::readCgiOutput( void )
{
	const int	bufferSize = 500;

	int			ret;
	char		buffer[bufferSize];

	close(_pipeFd[PIPE_WRITE]);
	do
	{
		ret = read(_pipeFd[PIPE_READ], buffer, bufferSize);
		if (ret > 0)
			_rawCgiOutput.append(buffer, ret);
	} while (ret > 0);
	close(_pipeFd[PIPE_READ]);
	if (ret < 0)
		throw serverError("Error reading cgi output");
}

void	Cgi::executeChildProcess( std::string requestedFilePath, std::vector<unsigned char>& body )
{
	const char*	cgiProgramPath = _rules.cgiPath.c_str();
	int		err;

	close(_pipeFd[PIPE_READ]);
	if (dup2(_pipeFd[PIPE_WRITE], STDOUT_FILENO) == -1)
		exit(500);
	close(_pipeFd[PIPE_WRITE]);
	createEnvp(requestedFilePath);
	createArgv(cgiProgramPath, stripExtraPathInfo(requestedFilePath));
	writeBodyToStdIn(body);
	err = execve(cgiProgramPath, _argv, _envp);
	if (err != -1)
		exit(0);
}
