/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 15:05:06 by pohl              #+#    #+#             */
/*   Updated: 2022/04/05 14:01:55 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/Cgi.hpp"
#include <unistd.h>
#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <exception>

void	Cgi::createArgv( const char* binPath, const char* filePath )
{
	_argv = (char**)malloc(sizeof(*_argv) * (3 + 1));
	if (_argv == NULL)
		exit(500);
	_argv[0] = strdup(binPath);
	if (_argv[0] == NULL)
		exit(500);
	_argv[1] = strdup(filePath);
	if (_argv[1] == NULL)
		exit(500);
	_argv[2] = 0;
}

void	Cgi::writeToEnvp( const std::map<std::string, std::string>& mapEnvp)
{
	std::string	tmp;
	size_t		index = 0;

	_envp = (char**)malloc(sizeof(*_envp) * (mapEnvp.size() + 1));
	if (_envp == NULL)
		exit(500);
	for (std::map<std::string, std::string>::const_iterator it = mapEnvp.begin();
			it != mapEnvp.end(); it++)
	{
		tmp = it->first;
		tmp += "=";
		tmp += it->second;
		_envp[index] = strdup(tmp.c_str());
		if (_envp[index] == NULL)
			exit(500);
		index++;
	}
	_envp[index] = 0;
}

void	Cgi::setFromHeader( const char* envpKey, string_map& envp,
		const char* headerKey, const string_map& header )
{
	string_map::const_iterator headerValue = header.find(headerKey);

	if (headerValue == header.end())
		return;
	envp[envpKey] = headerValue->second;
}

void	Cgi::setPathInfo( std::string& requestedFilePath, string_map& envp )
{
	char *documentRoot = get_current_dir_name();
	size_t	endOfScriptName = requestedFilePath.find("." + _rules.cgiExtension)
		+ _rules.cgiExtension.size() + 1;
	size_t	queryPosition = requestedFilePath.find('?');

	if (documentRoot == NULL)
		exit(500);
	envp["DOCUMENT_ROOT"] = documentRoot;
	free(documentRoot);
	if (queryPosition != std::string::npos)
	{
		envp["QUERY_STRING"] =
			requestedFilePath.substr(queryPosition + 1, std::string::npos);
		requestedFilePath.erase(queryPosition, std::string::npos);
	}
	envp["SCRIPT_NAME"] = requestedFilePath.substr(0, endOfScriptName);
	if (requestedFilePath[endOfScriptName] == '/')
	{
		envp["PATH_INFO"] =
			requestedFilePath.substr(endOfScriptName, std::string::npos);
		envp["PATH_TRANSLATED"] = envp["DOCUMENT_ROOT"] + envp["PATH_INFO"];
	}
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
	std::string body = "fname=Paul&lname=OHL";
	int			pipeFd[2];

	createPipe(pipeFd);
	if (write(pipeFd[PIPE_WRITE], body.c_str(), body.size())
			!= static_cast<ssize_t>(body.size()))
		exit(500);
	close(pipeFd[PIPE_WRITE]);
	if (dup2(pipeFd[PIPE_READ], STDIN_FILENO) == -1)
		exit(500);
	close(pipeFd[PIPE_READ]);
}

int	Cgi::createFork( void )
{
	int forkPid = fork();

	if (forkPid == -1)
	{
		throw std::exception(); // 500 Internal Server Error
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
		throw std::exception(); // 500 Internal Server Error
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
	size_t	extensionPosition = requestedFilePath
		.find("." + _rules.cgiExtension);
	size_t	exceptionSize = _rules.cgiExtension.size() + 1;

	requestedFilePath.erase(extensionPosition + exceptionSize,
		std::string::npos);
	return requestedFilePath.c_str();
}

std::string Cgi::popHeadersFromCgiOutput( void )
{
	const char*	possibleEndOfHeader[] = {
		"\b\n\b\n",
		"\r\n\r\n",
		"\n\n"
	};

	int			selectedEoh = 0;
	size_t		endOfHeaders;
	std::string rawHeaders;

	do
	{
		endOfHeaders = _rawCgiOutput.find(possibleEndOfHeader[selectedEoh]);
	} while (endOfHeaders == std::string::npos && ++selectedEoh < 3);
	if (endOfHeaders == std::string::npos)
		throw std::logic_error("No headers found in cgi output"); // error 500
	rawHeaders = _rawCgiOutput.substr(0, endOfHeaders);
	_rawCgiOutput.erase(0, endOfHeaders
			+ strlen(possibleEndOfHeader[selectedEoh]));
	return rawHeaders;
}

static std::pair<std::string, std::string>	makeHeaderPair(
		std::string& rawHeaders, size_t &nextHeaderPosition )
{
	std::pair<std::string, std::string>	headerPair;
	size_t	colonPosition = rawHeaders.find(':', nextHeaderPosition);
	size_t	endOfHeader = rawHeaders.find('\n', nextHeaderPosition);

	headerPair.first = rawHeaders.substr(nextHeaderPosition,
			colonPosition - nextHeaderPosition);
	colonPosition++;
	headerPair.second = rawHeaders.substr(colonPosition, endOfHeader - colonPosition);
	nextHeaderPosition = endOfHeader;
	if (endOfHeader != std::string::npos)
		nextHeaderPosition++;
	return headerPair;
}

bool	stringComparison( std::string str1, const char* str2, size_t index )
{
	return str1.compare(index, strlen(str2), str2) == 0;
}

static size_t	setHeader( response& response, std::string& rawHeaders,
		size_t& nextHeaderPosition )
{
	if (stringComparison(rawHeaders, "Content-Length", nextHeaderPosition))
		response.addFieldToHeaderMap(makeHeaderPair(rawHeaders, nextHeaderPosition));
	else if (stringComparison(rawHeaders, "Content-Type", nextHeaderPosition))
		response.addFieldToHeaderMap(makeHeaderPair(rawHeaders, nextHeaderPosition));
	else if (stringComparison(rawHeaders,  "Expires", nextHeaderPosition))
		response.addFieldToHeaderMap(makeHeaderPair(rawHeaders, nextHeaderPosition));
	else if (stringComparison(rawHeaders,  "Pragma", nextHeaderPosition))
		response.addFieldToHeaderMap(makeHeaderPair(rawHeaders, nextHeaderPosition));
	else
		response.addFieldToHeaderMap(makeHeaderPair(rawHeaders, nextHeaderPosition));
	return nextHeaderPosition;
}

void	Cgi::writeHeadersToResponse( std::string& rawHeaders,
		response& response )
{
	size_t	nextHeaderPosition = 0;

	while (nextHeaderPosition != std::string::npos)
	{
		if (stringComparison(rawHeaders, "Location", nextHeaderPosition))
		{
			_rules.redirectCode = 302;
			_rules.redirectUri
				= makeHeaderPair(rawHeaders, nextHeaderPosition).second;
		}
		else if (stringComparison(rawHeaders, "Status", nextHeaderPosition))
		{
			response.setStatusLine(atoi(makeHeaderPair(rawHeaders,
							nextHeaderPosition).second.c_str()));
		}
		else
			setHeader(response, rawHeaders, nextHeaderPosition);
	}
}
