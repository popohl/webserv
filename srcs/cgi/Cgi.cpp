/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:54 by pohl              #+#    #+#             */
/*   Updated: 2022/03/30 15:39:21 by pohl             ###   ########.fr       */
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

Cgi::Cgi( Rules& rules ): _rules(rules)
{
	return;
}

Cgi::~Cgi( void )
{
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
	char	requested_document[] = "/mnt/nfs/homes/pohl/Documents/42/webserv/srcs/testing/cgi_scripts/envp.php";
	char	request_type[] = "POST";

	char**	execveEnvp;
	char**	execveArgv;
	int		err;

	close(_pipeFd[0]);
	dup2(_pipeFd[1], STDOUT_FILENO);
	close(_pipeFd[1]);
	execveEnvp = createEnvp();
	execveArgv = createArgv(_rules.cgiPath.c_str(), requested_document);
	if (strcmp(request_type, "POST") == 0)
		writeBodyToStdIn();
	err = execve(_rules.cgiPath.c_str(), execveArgv, execveEnvp);
	if (err == -1)
		throw std::logic_error(strerror(errno)); // error 500
}
