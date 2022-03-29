/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childProcess.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 10:14:54 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 10:18:27 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <errno.h>
#include <exception>

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
	/* std::string	document_root = "DOCUMENT_ROOT="; */

	/* document_root += get_current_dir_name(); */
	envp = (char**)malloc(sizeof(*envp) * 6);
	envp[0] = strdup("REQUEST_METHOD=POST");
	envp[1] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	/* envp[2] = strdup("PATH_INFO=a"); */
	/* envp[3] = strdup(document_root.c_str()); */
	envp[2] = strdup("PATH_TRANSLATED=/mnt/nfs/homes/pohl/Documents/a");
	envp[3] = strdup("REDIRECT_STATUS=1");
	envp[4] = 0;
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

void	executeChildProcess( int pipeFd[2] )
{
	const char	programName[] = "/usr/bin/php";
	/* const char	programName[] = "/usr/bin/php-cgi"; */
	/* const char	programName[] = "/usr/bin/perl"; */
	/* const char	programName[] = "/usr/bin/python3"; */

	char**	execveEnvp;
	char**	execveArgv;
	int		err;

	dup2(pipeFd[1], STDOUT_FILENO);
	closePipe(pipeFd);
	execveEnvp = createEnvp();
	execveArgv = createArgv(programName, "/mnt/nfs/homes/pohl/Documents/b");
	writeFileToStdIn("/mnt/nfs/homes/pohl/Documents/c");
	err = execve(programName, execveArgv, execveEnvp);
	if (err == -1)
		throw std::logic_error(strerror(errno)); // error 500
}
