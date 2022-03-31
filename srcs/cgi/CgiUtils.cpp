/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 15:05:06 by pohl              #+#    #+#             */
/*   Updated: 2022/03/31 21:34:15 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/Cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <errno.h>
#include <exception>

void	Cgi::createArgv( const char* binPath, const char* filePath )
{
	_argv = (char**)malloc(sizeof(*_argv) * (3 + 1));
	_argv[0] = strdup(binPath);
	_argv[1] = strdup(filePath);
	_argv[2] = 0;
}

void	Cgi::writeToEnvp( std::map<std::string, std::string> mapEnvp)
{
	std::string	tmp;
	size_t		index = 0;

	_envp = (char**)malloc(sizeof(*_envp) * (mapEnvp.size() + 1));
	for (std::map<std::string, std::string>::iterator it = mapEnvp.begin();
			it != mapEnvp.end(); it++)
	{
		tmp = it->first;
		tmp += "=";
		tmp += it->second;
		_envp[index++] = strdup(tmp.c_str());
	}
	_envp[index++] = 0;
}

void	Cgi::createEnvp( void )
{
	std::map<std::string, std::string>	envp;

	/* document_root += get_current_dir_name(); */
	writeToEnvp(envp);
}

void	Cgi::writeBodyToStdIn( void )
{
	std::string body = "fname=Paul&lname=OHL";
	int			pipeFd[2];

	createPipe(pipeFd);
	if (write(pipeFd[PIPE_WRITE], body.c_str(), body.size())
			!= static_cast<ssize_t>(body.size()))
		std::logic_error("Could not write to cgi tmp file");
	close(pipeFd[PIPE_WRITE]);
	dup2(pipeFd[PIPE_READ], STDIN_FILENO);
	close(pipeFd[PIPE_READ]);
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

void	Cgi::freeEnvp( void )
{
	freeStringPointer(_envp);
	_envp = NULL;
}

void	Cgi::freeArgv( void )
{
	freeStringPointer(_argv);
	_argv = NULL;
}

void	Cgi::freeStringPointer( char** stringPtr )
{
	if (!stringPtr)
		return;
	for (size_t i = 0; stringPtr[i] != NULL; i++)
		free(stringPtr[i]);
	free(stringPtr);
}
