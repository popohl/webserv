/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:21 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 10:47:19 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static std::string	readCgiOutput( int pipeFd[2] )
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

std::string	executeCgi( void )
{
	int			pipeFd[2];
	int			forkPid;

	createPipe(pipeFd);
	forkPid = createFork();
	if (isChildProcess(forkPid))
	{
		executeChildProcess(pipeFd);
		exit(0);
	}
	waitpid(forkPid, NULL, WNOHANG);
	return readCgiOutput(pipeFd);
}
