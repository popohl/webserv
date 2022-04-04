/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             */
/*   Updated: 2022/04/04 19:41:06 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

iRequest * iRequest::createRequest(std::string &input, const std::vector<ServerNode *> & server)
{
	iRequest * result = NULL;
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
		//check httpVersion
	}

	//allocate memory
	if (method.length() && requestUri.length() && httpVersion.length())
	{
		input.erase(0, eraseLen + 2);
		if (method == "GET")
			result = new getRequest;
		else if (method == "POST")
			result = new postRequest;
		else if (method == "DELETE")
			result = new deleteRequest;
		if (result)
		{
			result->_server = &server;
			result->_requestURI = requestUri;
			result->_message.parseRequest(input);
		}
	}
	return result;
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
	if (!access(file.c_str(), F_OK | R_OK))
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

std::string iRequest::createFilePath()
{
	//check each location for the vector
	std::string filePath;
	const ServerNode * test = findServer();
	if (test)
	{
		const LocationRules * location = test->getLocationFromUrl(getRequestURI());

		if (location)
		{
			if (getRequestURI() == "/")
			{
				if (test->getServerRules().autoindex == true)
//display an autoindex;
					std::cout << "autoindex is on" << std::endl;
				else
					filePath = testIndexFile(location->root + "/", test->getServerRules().index);
			}
			else
				filePath = location->getPathFromLocation(getRequestURI());
		}
	}
	if (!filePath.length())
		throw fileNotFound();
	return (filePath);
}

std::string iRequest::createFileFromCgi( Rules& rules,
		std::string requestedFilePath, response& response )
{
	Cgi			cgi(rules, this);

	cgi.executeCgi(requestedFilePath);
	cgi.parseAndRemoveHeaders(response);
	return cgi.writeBodyToTmpFile();
}

std::string iRequest::testIndexFile(std::string root, const std::vector<std::string> & indexList)
{
	std::string file;
	for (std::vector<std::string>::const_iterator it = indexList.begin(); it != indexList.end(); it++)
	{
		file = root + *it;
		if (fileExists(file))
			break ;
	}
	return (file);
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

response getRequest::createResponse() {
	Rules rules;
	response response;

	rules.setValues(*findServer(), getRequestURI().c_str());
	
	if (!_message.containsHostField())
		response.setErrorMessage(400, rules);
	else if (!rules.isMethodAllowed(Rules::GET))
		response.setErrorMessage(405, rules);
	else
	{
		try
		{
			std::string filePath = createFilePath();
			if (filePath.length())
			{
				if (rules.isCgi(filePath))
					response.tryToOpenAndReadFile(createFileFromCgi(rules, filePath, response));
				else
					response.tryToOpenAndReadFile(filePath);
				response.addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Date", date()));
			}
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			response.setErrorMessage(404, rules);
		}
	}
	return response;
}

postRequest::postRequest()
{}

postRequest::~postRequest()
{}

response postRequest::createResponse() {

	Rules rules;
	rules.setValues(*findServer(), getRequestURI().c_str());
	response	response;
	std::string	postedFile;

	if (!_message.containsHostField())
		response.setErrorMessage(400, rules);
	else if (!rules.isMethodAllowed(Rules::POST))
		response.setErrorMessage(405, rules);
	else
	{
		postedFile = createPostedFilePath(rules.root, getRequestURI());
		std::ofstream file;
		file.open(postedFile.c_str());
		if (file.good())
		{
			file << _message._body;
			file.close();
			//set post default response if everything works
			response.setErrorMessage(201, rules);
			response.addFieldToHeaderMap(std::make_pair<std::string, std::string>("Location", getRequestURI()));
		}
		else
			response.setErrorMessage(400, rules);
	}
	return (response);


	//check content Type to know file information
	//Content Length or Transfer Encoding MUST be present in the header
	//POST creates a ressource or append it ? in the host server at the requestURI address
}

std::string postRequest::createPostedFilePath(const std::string & root, const std::string & requestURI)
{
	std::string::const_reverse_iterator it = root.rbegin();
	if (*it != '/')
		return (std::string(root + "/" + requestURI));
	else
		return (std::string(root + requestURI));
}

deleteRequest::deleteRequest()
{}

deleteRequest::~deleteRequest()
{}

response deleteRequest::createResponse() {

	response response;
	Rules rules;
	rules.setValues(*findServer(), getRequestURI().c_str());

	if (!_message.containsHostField())
		response.setErrorMessage(400, rules);
	else if (!rules.isMethodAllowed(Rules::DELETE))
		response.setErrorMessage(405, rules);
	else
	{
		std::string filePath(rules.root + getRequestURI());
		if (!remove(filePath.c_str()))
			response.setStatusLine(204);
		else
			response.setStatusLine(404);;
	}
	response.addFieldToHeaderMap(std::make_pair<std::string, std::string>("Date", date()));
	return response;
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
