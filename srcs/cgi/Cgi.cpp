/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:54 by pohl              #+#    #+#             */
/*   Updated: 2022/03/31 21:05:33 by pohl             ###   ########.fr       */
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

Cgi::Cgi( Rules& rules, const iRequest* request ): _rules(rules), _request(request)
{
	return;
}

Cgi::~Cgi( void )
{
	freeArgv();
	freeEnvp();
	return;
}

std::string	Cgi::executeCgi( void )
{
	int			forkPid;

	createPipe(_pipeFd);
	forkPid = createFork();
	if (isChildProcess(forkPid))
	{
		executeChildProcess();
		exit(0);
	}
	waitpid(forkPid, NULL, WNOHANG);
	return readCgiOutput();
}

std::string	Cgi::readCgiOutput( void )
{
	const int	bufferSize = 500;

	int			ret;
	char		buffer[bufferSize];
	std::string	cgiOutput;

	close(_pipeFd[PIPE_WRITE]);
	do
	{
		ret = read(_pipeFd[PIPE_READ], buffer, bufferSize);
		cgiOutput.append(buffer, ret);
	} while (ret > 0);
	close(_pipeFd[PIPE_READ]);
	if (ret < 0)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
	return cgiOutput;
}

void	Cgi::executeChildProcess( void )
{
	char	requested_document[] = "/mnt/nfs/homes/pohl/Documents/42/webserv/srcs/testing/cgi_scripts/envp.py";
	char	request_type[] = "POST";

	const char*	cgiProgramPath = _rules.cgiPath.c_str();
	int		err;

	close(_pipeFd[PIPE_READ]);
	dup2(_pipeFd[PIPE_WRITE], STDOUT_FILENO);
	close(_pipeFd[PIPE_WRITE]);
	execveEnvp = createEnvp();
	/* execveArgv = createArgv(_rules.cgiPath.c_str(), requested_document); */
	execveArgv = createArgv("/bin/python3", requested_document);
	if (strcmp(request_type, "POST") == 0)
		writeBodyToStdIn();
	/* err = execve(_rules.cgiPath.c_str(), execveArgv, execveEnvp); */
	err = execve("/bin/python3", execveArgv, execveEnvp);
	if (err == -1)
		throw std::logic_error(strerror(errno)); // error 500
}
