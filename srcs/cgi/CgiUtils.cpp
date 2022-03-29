/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 15:05:06 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 15:25:49 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/Cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <errno.h>
#include <exception>

char**	Cgi::createArgv( const char* binPath, const char* filePath )
{
	char** argv;

	argv = (char**)malloc(sizeof(*argv) * 3);
	argv[0] = strdup(binPath);
	argv[1] = strdup(filePath);
	argv[2] = 0;
	return argv;
}

char**	Cgi::generateEnvpPtrFromMap( std::map<std::string, std::string> envp)
{
	(void)envp;
	return NULL;
}

char**	Cgi::createEnvp( const Rules& rules )
{
	std::map<std::string, std::string>	envp;

	/* document_root += get_current_dir_name(); */
	/* envp = (char**)malloc(sizeof(*envp) * 6); */
	/* envp[0] = strdup("REQUEST_METHOD=POST"); */
	/* envp[1] = strdup("SERVER_PROTOCOL=HTTP/1.1"); */
	/* envp[2] = strdup("PATH_INFO=a"); */
	/* envp[3] = strdup(document_root.c_str()); */
	/* envp[2] = strdup("PATH_TRANSLATED=/mnt/nfs/homes/pohl/Documents/a"); */
	/* envp[3] = strdup("REDIRECT_STATUS=1"); */
	/* envp[4] = 0; */
	(void)rules;
	return generateEnvpPtrFromMap(envp);
}

void	Cgi::writeBodyToStdIn( void )
{
	std::string body = "name=paul&last_name=ohl";

	write(STDIN_FILENO, body.c_str(), body.size());
}

int	Cgi::createFork( void )
{
	int forkPid = fork();

	if (forkPid == -1)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
	return forkPid;
}

bool	Cgi::isChildProcess( int forkPid )
{
	return (forkPid != 0);
}

void	Cgi::createPipe( int pipeFd[2] )
{
	int returnValue = pipe(pipeFd);

	if (returnValue != 0)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
}

void	Cgi::closePipe( int pipeFd[2] )
{
	close(pipeFd[0]);
	close(pipeFd[1]);
}
