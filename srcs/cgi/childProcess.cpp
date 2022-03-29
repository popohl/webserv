/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childProcess.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 10:14:54 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 13:34:16 by pohl             ###   ########.fr       */
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

static char**	generateEnvpPtrFromMap( std::map<std::string, std::string> envp)
{
	(void)envp;
	return NULL;
}

static char**	createEnvp( const Rules& rules )
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

static void	writeBodyToStdIn( void )
{
	std::string body = "name=paul&last_name=ohl";

	write(STDIN_FILENO, body.c_str(), body.size());
}

void	executeChildProcess( const Rules &rules, int pipeFd[2] )
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
