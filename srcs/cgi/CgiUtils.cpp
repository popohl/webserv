/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 15:05:06 by pohl              #+#    #+#             */
/*   Updated: 2022/03/30 15:36:25 by pohl             ###   ########.fr       */
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
	char** charEnvp = (char**)malloc(sizeof(*charEnvp) * 6);
	charEnvp[0] = strdup("REQUEST_METHOD=POST");
	charEnvp[1] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	/* charEnvp[2] = strdup("PATH_INFO=a"); */
	/* charEnvp[3] = strdup(document_root.c_str()); */
	charEnvp[2] = strdup("PATH_TRANSLATED=/mnt/nfs/homes/pohl/Documents/a");
	charEnvp[3] = strdup("REDIRECT_STATUS=1");
	charEnvp[4] = 0;
	return charEnvp;
}

char**	Cgi::createEnvp( void )
{
	std::map<std::string, std::string>	envp;

	/* document_root += get_current_dir_name(); */
	return generateEnvpPtrFromMap(envp);
}

void	Cgi::writeBodyToStdIn( void )
{
	/* std::string body = "name=paul&last_name=ohl"; */

	/* write(0, body.c_str(), body.size()); */
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

void	Cgi::createPipe( void )
{
	int returnValue = pipe(_pipeFd);

	if (returnValue != 0)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
}
