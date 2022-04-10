/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             */
//   Updated: 2022/04/09 18:09:11 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "responses/httpExceptions.hpp"
#include "requests/requests.hpp"
#include "cgi/Cgi.hpp"
#include <cstddef>
#include <cstring>
#include <ctime>
#include <sstream>
#include <string.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include "configParsing/Rules.hpp"
#include <sys/types.h>
#include <dirent.h>

void		iRequest::printRequest()
{
	std::cout << "requestURI is " + getRequestURI() << std::endl;
	for (std::map<std::string, std::string>::iterator it = _message._header.begin();
		 it != _message._header.end();
		 it++)
		std::cout << "[" << it->first << "] " << it->second << std::endl;
}

iRequest * iRequest::createRequest(std::vector<char> &data, const std::vector<ServerNode *> & server)
{
	iRequest * result = NULL;
	std::string input(data.begin(), data.end());
	std::string method, requestUri, httpVersion;

	//isolate first line and three string tokens
	size_t eraseLen = input.find("\r\n");
	if (eraseLen != std::string::npos)
	{
		std::string requestLine(input, 0, eraseLen);
		//Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
		method = eatWord(requestLine);
		// expect an URI or replace it with / if field is empty
		requestUri = eatWord(requestLine);
		if (requestUri.find("HTTP") != std::string::npos)
		{
			httpVersion = requestUri;
			requestUri = std::string("/");
		}
		else
			httpVersion = eatWord(requestLine);
	}
	result = allocateRequest(method, requestUri, httpVersion);
	data.erase(data.begin(), data.begin() + eraseLen + 2);
	result->_server = &server;
	result->_requestURI = requestUri;
	result->_httpVersion = httpVersion;
	result->_message.parseRequest(data);
	return result;
}

iRequest * iRequest::allocateRequest(const std::string & method, const std::string & requestURI, const std::string & httpVersion)
{
	if (methodIsValid(method) && requestURIIsValid(requestURI) && httpVersionIsValid(httpVersion))
	{
		if (method == "GET")
			return (new getRequest);
		else if (method == "POST")
			return (new postRequest);
		else if (method == "DELETE")
			return (new deleteRequest);
	}
	return (new errorRequest);
}

bool iRequest::methodIsValid(const std::string &method)
{
	if (method == "GET" || method == "POST" || method == "DELETE")
		return (true);
	else
		return (false);
}

bool iRequest::requestURIIsValid(const std::string &requestURI)
{
	if ((*requestURI.begin() == '/') && (requestURI.length() < 2048))
		return (true);
	else
		return (false);
}

bool	iRequest::httpVersionIsValid(const std::string &httpVersion)
{
	if (httpVersion == "HTTP/1.0" || httpVersion == "HTTP/1.1")
		return (true);
	else
		return (false);
}

iRequest::~iRequest()
{}

bool iRequest::receivingisDone()
{
	if (_message._headerFinished && _message._bodyFinished)
		return (true);
	else
		return (false);
}

std::string iRequest::eatWord(std::string & line)
{
	//eats a word and remove spaces after
	size_t endOfWord = line.find(" ");
	std::string word(line, 0, endOfWord);

	line.erase(0, endOfWord);
	for (std::string::iterator it = line.begin(); *it == ' '; it = line.begin())
		line.erase(it);
	return (word);
}

const std::string & iRequest::getRequestURI()
{
	return (_requestURI);
}

bool fileExists(std::string file)
{
	if (!access(file.c_str(), F_OK))
		return (true);
	else
		return (false);
}

bool iRequest::containsPort(std::string hostname)
{
	size_t portStart(hostname.rfind(":"));
	if (portStart != std::string::npos)
	{
		for (size_t index = 1; (portStart + index < hostname.length()) && (index < 6); index++)
		{
			if (!isdigit(hostname[portStart + index]))
				return (false);
		}
		return (true);
	}
	else
		return (false);
}

std::string iRequest::createFilePath( Rules& rules )
{
	std::string filePath;
	filePath = rules.getPathFromLocation(getRequestURI());
	if ((*(filePath.rbegin())) == '/')
		filePath = testIndexFile(filePath, rules.index);
	if (!filePath.length())
		throw httpError(404, "Requested file not found");
	return (filePath);
}

std::string iRequest::createFileFromCgi( Rules& rules,
		std::string requestedFilePath, response& response )
{
	Cgi	cgi(rules, this);
	int	status;
	std::cout << "path in cgi : " << requestedFilePath << std::endl;
	cgi.executeCgi(requestedFilePath, _message._body);
	status = cgi.parseAndRemoveHeaders(response);
	if (status < 300)
	{
		response.setStatusLine(status);
		return cgi.writeBodyToTmpFile();
	}
	throw httpError(status, "Status code set by cgi");
}

std::string iRequest::testIndexFile(std::string root, const std::vector<std::string> & indexList)
{
	for (std::vector<std::string>::const_iterator it = indexList.begin(); it != indexList.end(); it++)
	{
		if (fileExists(root + *it))
			return root + *it;
	}
	return (root);
}

ServerNode * iRequest::findServer()
{
	std::string host = _message._header["Host"];
	std::string serverName ;

	for (std::vector<ServerNode *>::const_iterator it = _server->begin(); it != _server->end(); it++)
	{
		ServerRules tmpServerRules = (*it)->getServerRules();
		int port = tmpServerRules.listenPort;
		for (std::vector<std::string>::iterator names = tmpServerRules.serverName.begin(); names != tmpServerRules.serverName.end(); names++)
		{
			serverName = *names;

			if (containsPort(host))
				serverName += (":" + to_string(port));
			if (serverName == host)
				return (*it);
		}
	}
	return (*(_server->begin()));
}

getRequest::getRequest()
{}

getRequest::~getRequest()
{}

bool	isFolder( const char* filePath )
{
	DIR* directory = opendir(filePath);
	if (directory)
	{
		closedir(directory);
		return true;
	}
	return false;
}

response getRequest::createResponse() {
	Rules rules;
	response response;
	
	rules.setValues(*findServer(), getRequestURI().c_str());
	if (!rules.redirectCode)
	{
		try
		{
			checkRequestError(rules);
			std::string filePath = createFilePath(rules);
			response.addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Date", date()));
			response.setStatusLine(200);
			if (isFolder(filePath.c_str()))
			{
				if (*(filePath.rbegin()) != '/')
					filePath.push_back('/');
				if (rules.autoindex)
					response.createAutoindexResponse(filePath);
				else
					response.setErrorMessage(403, rules);
				return response;
			}
			else if (rules.isCgi(filePath))
				filePath = createFileFromCgi(rules, filePath, response);
			response.tryToOpenFile(filePath);
		}
		catch (httpError& e) {
			response.setErrorMessage(e.statusCode(), rules);
		}
		catch (std::exception& e) {
			response.setErrorMessage(500, rules);
		}
	}
	else
	{
		std::string newUri = getRequestURI().substr(rules.locationPath.size(), std::string::npos);
		response.addFieldToHeaderMap(std::make_pair("Location", "https://" + rules.redirectUri + "/" + newUri));
		response.setErrorMessage(rules.redirectCode, rules);
	}
	return response;
}

void		getRequest::checkRequestError(const Rules & rules)
{
	if (!_message.containsHostField())
		throw httpError(400);
	else if (!rules.isMethodAllowed(Rules::GET))
		throw httpError(405);
}

std::string getRequest::printType()
{
	return ("GET");
}

postRequest::postRequest()
{}

postRequest::~postRequest()
{}

response postRequest::createResponse()
{
	response	response;
	Rules rules;
	rules.setValues(*findServer(), getRequestURI().c_str());
	try
	{
		checkRequestError(rules);
		std::string filePath = createFilePath(rules);
		if (rules.isCgi(filePath))
		{
			response.setStatusLine(200);
			response.tryToOpenFile(createFileFromCgi(rules, filePath, response));
		}
		else
		{
			filePath = rules.getPathFromLocation(getRequestURI());
			createPostedFile(filePath);
			response.tryToOpenFile(filePath);
			response.setStatusLine(201);
			response.addFieldToHeaderMap(std::make_pair<std::string, std::string>("Location", getRequestURI()));
		}
		response.addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Date", date()));
	}
	catch (httpError& e) {
		response.setErrorMessage(e.statusCode(), rules);
	}
	catch (std::exception& e) {
		response.setErrorMessage(500, rules);
	}
	return (response);
}

void	postRequest::checkRequestError(const Rules & rules)
{
	if (!_message.containsHostField())
		throw httpError(400);
	else if (!rules.isMethodAllowed(Rules::POST))
		throw httpError(405);
	else if (!(static_cast<int>(_message._body.size()) < rules.clientMaxBodySize))
		throw httpError(413);
}

void	postRequest::createPostedFile(const std::string & path)
{
	std::ofstream file(path.c_str(), std::ofstream::app);
	if (file.good())
	{
		file.write(&*_message._body.begin(), _message._body.size());
		file.close();
	}
	else
		throw serverError(400);

}

std::string postRequest::printType()
{
	return ("POST");
}

deleteRequest::deleteRequest()
{}

deleteRequest::~deleteRequest()
{}

response deleteRequest::createResponse() {

	response response;
	Rules rules;
	rules.setValues(*findServer(), getRequestURI().c_str());

	try
	{
		checkRequestError(rules);
		std::string filePath = rules.getPathFromLocation(getRequestURI());
		if (!remove(filePath.c_str()))
			response.setStatusLine(204);
		else
			response.setErrorMessage(200, rules);
		response.addFieldToHeaderMap(std::make_pair<std::string, std::string>("Date", date()));
	}
	catch (httpError& e) {
		response.setErrorMessage(e.statusCode(), rules);
	}
	catch (std::exception& e) {
		response.setErrorMessage(500, rules);
	}
	return response;
}

void	deleteRequest::checkRequestError(const Rules & rules)
{
	if (!_message.containsHostField())
		throw httpError(400);
	else if (!rules.isMethodAllowed(Rules::DELETE))
		throw httpError(405);
}

errorRequest::errorRequest()
{}

errorRequest::~errorRequest()
{}

response errorRequest::createResponse()
{
	response response;
	Rules rules;
	rules.setValues(*findServer(), getRequestURI().c_str());

	if (getRequestURI().length() >= 2048)
		response.setErrorMessage(413, rules);
	else if (iRequest::requestURIIsValid(getRequestURI()) && iRequest::httpVersionIsValid(_httpVersion))
		response.setErrorMessage(405, rules);
	else if (!requestURIIsValid(getRequestURI()))
		response.setErrorMessage(400, rules);
	else
		response.setErrorMessage(505, rules);
	return (response);
}

std::string deleteRequest::printType()
{
	return ("DELETE");
}



std::string errorRequest::printType()
{
	return ("ERROR");
}


const char *days[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

std::string date()
{
	time_t now = time(0);
	tm * gmt = gmtime(&now);

	std::string result;
	std::stringstream tmp;
	tmp << " " << days[gmt->tm_wday];
	tmp << ", " << gmt->tm_mday;
	tmp << " " << months[gmt->tm_mon];
	tmp << " " << 1900 + gmt->tm_year;
	tmp << " " << gmt->tm_hour;
	tmp << ":" << gmt->tm_min;
	tmp << ":" << gmt->tm_sec;

	result += tmp.str();
	result += " GMT";
	return (result);
}
