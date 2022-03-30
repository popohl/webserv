/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:54 by pohl              #+#    #+#             */
/*   Updated: 2022/03/30 19:02:38 by pohl             ###   ########.fr       */
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

Cgi::Cgi( Rules& rules ): _rules(rules), _envp(NULL), _argv(NULL)
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

	createPipe();
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

	close(_pipeFd[1]);
	do
	{
		ret = read(_pipeFd[0], buffer, bufferSize);
		cgiOutput.append(buffer, ret);
	} while (ret > 0);
	close(_pipeFd[0]);
	if (ret < 0)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
	return cgiOutput;
}

void	Cgi::executeChildProcess( void )
{
	/* const char	requested_document[] = "/mnt/nfs/homes/pohl/Documents/b"; */
	/* const char	request_type[] = "POST"; */

	const char*	cgiProgramPath = _rules.cgiPath.c_str();
	int		err;

	std::cout << "Using program: " << cgiProgramPath << std::endl;
	close(_pipeFd[0]);
	dup2(_pipeFd[1], STDOUT_FILENO);
	close(_pipeFd[1]);
	createEnvp();
	/* createArgv(cgiProgramPath, requested_document); */
	createArgv("/bin/cat", "-e");
	/* if (strcmp(request_type, "POST") == 0) */
	/* 	writeBodyToStdIn(); */
	std::string body = "name=paul&last_name=ohl";

	write(2, body.c_str(), body.size());
	err = execve("/bin/cat", _argv, _envp);
	if (err == -1)
		throw std::logic_error(strerror(errno)); // error 500
}
