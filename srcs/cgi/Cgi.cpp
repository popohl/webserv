/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:54 by pohl              #+#    #+#             */
/*   Updated: 2022/04/03 17:26:00 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/Cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <errno.h>
#include <exception>

Cgi::Cgi( Rules& rules, const iRequest* request ):
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

void	Cgi::executeCgi( std::string requestedFilePath )
{
	int			forkPid;
	int			returnValue;

	std::cout << "Uri requested: " << requestedFilePath << std::endl;
	createPipe(_pipeFd);
	forkPid = createFork();
	if (isChildProcess(forkPid))
	{
		executeChildProcess(requestedFilePath);
		exit(0);
	}
	waitpid(forkPid, &returnValue, 0);
	std::cout << "WEXITSTATUS(returnValue): "
		<< WEXITSTATUS(returnValue) << std::endl;
	readCgiOutput();
}

void	Cgi::parseAndRemoveHeaders( response& response )
{
	std::string rawHeaders = popHeadersFromCgiOutput();

	writeHeadersToResponse(rawHeaders, response);
}

std::string Cgi::writeBodyToTmpFile( void )
{
	std::string	fileName("/tmp/webserv-tmp-cgi");
	int			fileFd = open(fileName.c_str(), O_WRONLY | O_CREAT,
			S_IRUSR | S_IRGRP | S_IROTH);

	if (fileFd == -1)
		throw std::logic_error("Could not open file for writing");
	if (write(fileFd, _rawCgiOutput.c_str(), _rawCgiOutput.size())
			!= static_cast<ssize_t>(_rawCgiOutput.size()))
		throw std::logic_error("Problem writing to file");
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
		_rawCgiOutput.append(buffer, ret);
	} while (ret > 0);
	close(_pipeFd[PIPE_READ]);
	if (ret < 0)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
}

void	Cgi::executeChildProcess( std::string requestedFilePath )
{
	const char*	cgiProgramPath = _rules.cgiPath.c_str();
	int		err;

	/* std::cout << "Cgi infos:" << std::endl << "cgiProgramPath: " */
	/* 	<< cgiProgramPath << ", requestedFilePath: " << requestedFilePath */
	/* 	<< std::endl; */
	close(_pipeFd[PIPE_READ]);
	dup2(_pipeFd[PIPE_WRITE], STDOUT_FILENO);
	close(_pipeFd[PIPE_WRITE]);
	createEnvp(requestedFilePath);
	createArgv(cgiProgramPath, stripExtraPathInfo(requestedFilePath));
	if (isPostRequest())
		writeBodyToStdIn();
	err = execve(cgiProgramPath, _argv, _envp);
	perror("In execve:");
	if (err == -1)
		exit(1); // error
}
