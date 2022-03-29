/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:54 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 15:26:16 by pohl             ###   ########.fr       */
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
		executeChildProcess(rules, pipeFd);
		exit(0);
	}
	waitpid(forkPid, NULL, WNOHANG);
	return readCgiOutput(pipeFd);
}

std::string	Cgi::readCgiOutput( int pipeFd[2] )
{
	const int	bufferSize = 500;

	int			ret;
	char		buffer[bufferSize];
	std::string	cgiOutput;

	close(pipeFd[1]);
	do
	{
		ret = read(pipeFd[0], buffer, bufferSize);
		cgiOutput.append(buffer, ret);
	} while (ret > 0);
	close(pipeFd[0]);
	if (ret < 0)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
	return cgiOutput;
}

void	Cgi::executeChildProcess( const Rules &rules, int pipeFd[2] )
{
	char	requested_document[] = "/mnt/nfs/homes/pohl/Documents/b";
	char	request_type[] = "POST";

	char**	execveEnvp;
	char**	execveArgv;
	int		err;

	dup2(pipeFd[1], STDOUT_FILENO);
	closePipe(pipeFd);
	execveEnvp = createEnvp(rules);
	execveArgv = createArgv(rules.cgiPath.c_str(), requested_document);
	if (strcmp(request_type, "POST") == 0)
		writeBodyToStdIn();
	err = execve(rules.cgiPath.c_str(), execveArgv, execveEnvp);
	if (err == -1)
		throw std::logic_error(strerror(errno)); // error 500
}
