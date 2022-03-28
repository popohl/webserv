/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:21 by pohl              #+#    #+#             */
/*   Updated: 2022/03/28 10:15:36 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define STD_IN 0
#define STD_OUT 1

static char**	createArgv( const char* binPath, const char* filePath )
{
	char** env;

	env = (char**)malloc(sizeof(*env) * 2);
	env[0] = strdup(binPath);
	env[1] = strdup(filePath);
	return env;
}

static char**	createEnvp( void )
{
	char** env;

	env = (char**)malloc(sizeof(*env) * 3);
	env[0] = strdup("REQUEST_METHOD=GET");
	env[1] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[2] = strdup("PATH_INFO=/");
	return env;
}

static void	writeFileToStdIn( const char* pathToFile )
{
	int	fd = open(pathToFile, O_RDONLY);

	if (fd == -1)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
	if (dup2(fd, STD_IN) == -1)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
}

static void	executeChildProcess( void )
{
	char**	execveEnvp;
	char**	execveArgv;

	execveEnvp = createEnvp();
	execveArgv = createArgv("/bin/cat", "./Makefile");
	writeFileToStdIn("./Makefile");
	execve("/bin/cat", execveArgv, execveEnvp);
}

std::string	executeCgi( void )
{
	int			pipeFd[2];
	int			forkPid;
	std::string	cgiResult;

	createPipe(pipeFd);
	forkPid = createFork();
	// if child process
	if (isChildProcess(forkPid))
		executeChildProcess();
	// if parent
	else
	{
	//		wait for cgi execution
	//		maybe parse the cgi header? if so set the http header
	//		return result string
	}
	return cgiResult;
}
