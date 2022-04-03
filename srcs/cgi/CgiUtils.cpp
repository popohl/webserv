/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 15:05:06 by pohl              #+#    #+#             */
/*   Updated: 2022/04/02 12:48:51 by pohl             ###   ########.fr       */
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

void	Cgi::writeToEnvp( const std::map<std::string, std::string>& mapEnvp)
{
	std::string	tmp;
	size_t		index = 0;

	_envp = (char**)malloc(sizeof(*_envp) * (mapEnvp.size() + 1));
	for (std::map<std::string, std::string>::const_iterator it = mapEnvp.begin();
			it != mapEnvp.end(); it++)
	{
		tmp = it->first;
		tmp += "=";
		tmp += it->second;
		_envp[index++] = strdup(tmp.c_str());
	}
	_envp[index] = 0;
}

void	Cgi::setFromHeader( const char* envpKey, string_map& envp,
		const char* headerKey, const string_map& header )
{
	string_map::const_iterator headerValue = header.find(headerKey);

	if (headerValue == header.end())
		return;
	envp[envpKey] = headerValue->first;
}

void	Cgi::setPathInfo( std::string& requestedFilePath, string_map& envp )
{
	char *documentRoot = get_current_dir_name();
	size_t	endOfScriptName = requestedFilePath.find("." + _rules.cgiExtension)
		+ _rules.cgiExtension.size() + 1;
	size_t	tmp;

	envp["DOCUMENT_ROOT"] = documentRoot;
	free(documentRoot);
	tmp = requestedFilePath.find('?');
	if (tmp == std::string::npos)
		envp["QUERY_STRING"] = "";
	else
	{
		envp["QUERY_STRING"] = requestedFilePath.substr(tmp, std::string::npos);
		requestedFilePath.erase(tmp, std::string::npos);
	}
	envp["SCRIPT_NAME"] = requestedFilePath.substr(0, endOfScriptName);
	envp["PATH_INFO"] = requestedFilePath
		.substr(endOfScriptName, std::string::npos);
	envp["PATH_TRANSLATED"] = envp["DOCUMENT_ROOT"] + envp["PATH_INFO"];
}

void	Cgi::createEnvp( std::string requestedFilePath )
{
	string_map			envp;
	const string_map&	header = _request->_message._header;

	envp["REQUEST_METHOD"] = isPostRequest() ? "POST" : "GET";
	envp["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp["SERVER_PROTOCOL"] = "HTTP/1.0";
	envp["SERVER_SOFTWARE"] = "webserv/0.42.118";
	setFromHeader("CONTENT_LENGTH", envp, "Content-Length", header);
	setFromHeader("CONTENT_TYPE", envp, "Content-Type", header);
	setFromHeader("HTTP_ACCEPT", envp, "Accept", header);
	setFromHeader("HTTP_REFERER", envp, "Referer", header);
	setFromHeader("HTTP_USER_AGENT", envp, "User-Agent", header);
	setFromHeader("SERVER_NAME", envp, "Host", header);
	envp["REQUEST_URI"] = _request->getRequestURI();
	envp["SERVER_PORT"] = _rules.listenPort;
	// "REMOTE_ADDR" -> client address
	// "REMOTE_HOST" -> client name
	setPathInfo(requestedFilePath, envp);
	writeToEnvp(envp);
}

void	Cgi::writeBodyToStdIn( void )
{
	std::cout << "writeBodyToStdIn got called" << std::endl;
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
	return (forkPid == 0);
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

bool	Cgi::isPostRequest( void )
{
	const postRequest* tmp;

	tmp = dynamic_cast<const postRequest*>(_request);
	if (tmp == NULL)
		return false;
	return true;
}

const char*	Cgi::stripExtraPathInfo( std::string &requestedFilePath )
{
	requestedFilePath.erase(requestedFilePath.find_first_of('.') + 1
			+ _rules.cgiExtension.size(), std::string::npos);
	return requestedFilePath.c_str();
}
