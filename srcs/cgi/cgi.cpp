/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:21 by pohl              #+#    #+#             */
/*   Updated: 2022/03/28 15:00:42 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <errno.h>

static char**	createArgv( const char* binPath, const char* filePath )
{
	char** argv;

	argv = (char**)malloc(sizeof(*argv) * 3);
	argv[0] = strdup(binPath);
	argv[1] = strdup(filePath);
	argv[2] = 0;
	return argv;
}

static char**	createEnvp( void )
{
	char**		envp;
	std::string	document_root = "DOCUMENT_ROOT=";

	document_root += get_current_dir_name();
	envp = (char**)malloc(sizeof(*envp) * 6);
	envp[0] = strdup("REQUEST_METHOD=POST");
	envp[1] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	envp[2] = strdup("PATH_INFO=a");
	envp[3] = strdup(document_root.c_str());
	envp[4] = strdup("PATH_TRANSLATED=/mnt/nfs/homes/pohl/Documents/a");
	envp[5] = strdup("REDIRECT_STATUS=1");
	envp[6] = 0;
	return envp;
}

static void	writeFileToStdIn( const char* pathToFile )
{
	int	fd = open(pathToFile, O_RDONLY);

	if (fd == -1)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
}

static void	executeChildProcess( void )
{
	char**	execveEnvp;
	char**	execveArgv;
	int		err;
	char	program[] = "/usr/bin/php-cgi";
	/* char	program[] = "/mnt/nfs/homes/pohl/Documents/42/webserv/research/ubuntu_cgi_tester"; */

	std::cout << "Using " << program << std::endl;
	execveEnvp = createEnvp();
	execveArgv = createArgv(program, "/mnt/nfs/homes/pohl/Documents/b");
	writeFileToStdIn("/mnt/nfs/homes/pohl/Documents/c");
	err = execve(program, execveArgv, execveEnvp);
	if (err == -1)
		throw std::logic_error(strerror(errno));
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
