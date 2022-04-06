/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 15:05:06 by pohl              #+#    #+#             */
/*   Updated: 2022/04/06 15:36:33 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responses/httpExceptions.hpp"
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
		exit(503);
	_argv[0] = strdup(binPath);
	if (_argv[0] == NULL)
		exit(503);
	_argv[1] = strdup(filePath);
	if (_argv[1] == NULL)
		exit(503);
	_argv[2] = 0;
}

void	Cgi::writeToEnvp( const std::map<std::string, std::string>& mapEnvp)
{
	std::string	tmp;
	size_t		index = 0;

	_envp = (char**)malloc(sizeof(*_envp) * (mapEnvp.size() + 1));
	if (_envp == NULL)
		exit(503);
	for (std::map<std::string, std::string>::const_iterator it = mapEnvp.begin();
			it != mapEnvp.end(); it++)
	{
		tmp = it->first;
		tmp += "=";
		tmp += it->second;
		_envp[index] = strdup(tmp.c_str());
		if (_envp[index] == NULL)
			exit(503);
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
		exit(503);
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
		exit(503);
	close(pipeFd[PIPE_WRITE]);
	if (dup2(pipeFd[PIPE_READ], STDIN_FILENO) == -1)
		exit(503);
	close(pipeFd[PIPE_READ]);
}

int	Cgi::createFork( void )
{
	int forkPid = fork();

	if (forkPid == -1)
		throw serverError(503, "Fork failed");
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
		throw serverError(503, "Pipe failed");
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
		throw serverError("No headers found in cgi output");
	rawHeaders = _rawCgiOutput.substr(0, endOfHeaders);
	_rawCgiOutput.erase(0, endOfHeaders
			+ strlen(possibleEndOfHeader[selectedEoh]));
	return rawHeaders;
}

static std::string getHeaderValue( std::string& rawHeaders, size_t& nextHeaderPosition )
{
	size_t		valueBeginPos = rawHeaders.find(':', nextHeaderPosition) + 1;
	size_t		endOfHeader = rawHeaders.find('\n', nextHeaderPosition);
	std::string headerValue = rawHeaders.substr(valueBeginPos, endOfHeader - valueBeginPos);

	std::cout << "Header value: " << headerValue << std::endl;
	nextHeaderPosition = endOfHeader;
	if (endOfHeader != std::string::npos)
		nextHeaderPosition++;
	return headerValue;
}

bool	stringComparison( std::string str1, const char* str2, size_t index )
{
	unsigned int sz = strlen(str2);

    for (unsigned int i = 0; i < sz; i++)
        if (tolower(str1[i + index]) != tolower(str2[i]))
            return false;
    return true;
}

static size_t	setHeaderField( response& response, std::string& rawHeaders,
		size_t& nextHeaderPosition )
{
	if (stringComparison(rawHeaders, "Content-Length", nextHeaderPosition))
		response.addFieldToHeaderMap(std::make_pair("Content-Length", getHeaderValue(rawHeaders, nextHeaderPosition)));
	else if (stringComparison(rawHeaders, "Content-Type", nextHeaderPosition))
		response.addFieldToHeaderMap(std::make_pair("Content-Type", getHeaderValue(rawHeaders, nextHeaderPosition)));
	else if (stringComparison(rawHeaders, "Expires", nextHeaderPosition))
		response.addFieldToHeaderMap(std::make_pair("Expires", getHeaderValue(rawHeaders, nextHeaderPosition)));
	else if (stringComparison(rawHeaders, "Pragma", nextHeaderPosition))
		response.addFieldToHeaderMap(std::make_pair("Pragma", getHeaderValue(rawHeaders, nextHeaderPosition)));
	else
		throw serverError(503, "Unexpected field found in cgi header");
	return nextHeaderPosition;
}

int		Cgi::writeHeadersToResponse( std::string& rawHeaders,
		response& response )
{
	size_t	nextHeaderPosition = 0;
	int		status = 200;

	while (nextHeaderPosition != std::string::npos)
	{
		if (stringComparison(rawHeaders, "Location", nextHeaderPosition))
		{
			_rules.redirectCode = 302;
			_rules.redirectUri
				= getHeaderValue(rawHeaders, nextHeaderPosition);
			status = 302;
		}
		else if (stringComparison(rawHeaders, "Status", nextHeaderPosition))
		{
			response.setStatusLine(atoi(getHeaderValue(rawHeaders,
							nextHeaderPosition).c_str()));
		}
		else
			setHeaderField(response, rawHeaders, nextHeaderPosition);
	}
	return status;
}
