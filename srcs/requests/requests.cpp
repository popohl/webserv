// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
/*   requests.cpp                                       :+:      :+:    :+:   */
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
/*   Updated: 2022/04/01 15:34:11 by pohl             ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

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

getRequest::getRequest() {}
postRequest::postRequest() {}
deleteRequest::deleteRequest() {}

iRequest * iRequest::createRequest(std::string &input, const std::vector<ServerNode *> & server) //be able to remove first line from buffer
{
	iRequest * result = NULL;
	std::string method, requestUri, httpVersion;

	//isolate first line and three string tokens
	size_t eraseLen = input.find("\r\n");
	if (eraseLen != std::string::npos)
	{
		std::string requestLine(input, 0, eraseLen);
		//Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
		std::cout << requestLine << std::endl;
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

bool iRequest::receivingisDone()
{
	if (_message._headerFinished && _message._bodyFinished)
		return (true);
	else
		return (false);
}

std::string eatWord(std::string & line)
{
	size_t endOfWord = line.find(" ");
	std::string word(line, 0, endOfWord);

	line.erase(0, endOfWord);
	for (std::string::iterator it = line.begin(); *it == ' '; it = line.begin())
		line.erase(it);
	return (word);
}

const std::string & iRequest::getRequestURI() const
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

bool containsPort(std::string hostname)
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
	/* std::cout << "findServer result : " << test << std::endl; */
	if (test)
	{
		const LocationRules * location = test->getLocationFromUrl(getRequestURI());

		if (location)
		{
			if (getRequestURI() == "/")
			{
				std::cout << "got here" << std::endl;
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
		std::string requestedFilePath )
{
	Cgi			cgi(rules, this);
	std::string rawCgiOutput(cgi.executeCgi(requestedFilePath));

	/* std::cout << "Cgi output: --->" << std::endl << rawCgiOutput << std::endl */
	/* 	<< "<---" << std::endl; */
	return "";
	/* return rawCgiOutput; */
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

response getRequest::createResponse() {
	response	response;
	Rules		rules;

	if (_message._header.find("Host") == _message._header.end())
	{
		response.setErrorMessage(400);
		return (response);
	}
	rules.setValues(*findServer(), getRequestURI());
	if (!rules.isMethodAllowed(Rules::GET))
	{
		response.setErrorMessage(405);
		return (response);
	}
	try {
		std::string filePath = createFilePath();
		if (filePath.length())
		{
			if (rules.isCgi(filePath))
			/* if (false) */
				response.tryToOpenAndReadFile(createFileFromCgi(rules, filePath));
			else
			{
				response.addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Date", date()));
				response.tryToOpenAndReadFile(filePath);
			}
		}
	}
	catch (std::exception &e){
		//file not found
		std::cout << e.what() << std::endl;
		response.setErrorMessage(404);
		return (response);
	}
	response.setStatusLine(200);	
	return response;
}


response postRequest::createResponse() {
	response	response;
	std::string	postedFile;

	if (_message._header.find("Host") == _message._header.end())
	{
		response.setErrorMessage(400);
		return (response);
	}

	ServerNode * server = findServer();
	if (server)
	{
		const LocationRules * location = server->getLocationFromUrl(getRequestURI());
		if (!location)
		{
			response.setErrorMessage(404);
			return (response);
		}
		else if (location && !(location->allowedMethod & LocationRules::POST))
		{
			response.setErrorMessage(405);
			return (response);
		}
		else
		{
			postedFile = location->root + getRequestURI();
			std::ofstream file;
			file.open(postedFile.c_str());
			file << _message._body;
			file.close();
			response.setErrorMessage(200);
			return (response);
		}
	}
	response.setErrorMessage(400);
	return (response);
	

	//check content Type to know file information
	//Content Length or Transfer Encoding MUST be present in the header

	//POST creates a ressource or append it ? in the host server at the requestURI address


	/*
	  The action performed by the POST method might not result in a
	  resource that can be identified by a URI. In this case, either 200
	  (OK) or 204 (No Content) is the appropriate response status,
	  depending on whether or not the response includes an entity that
	  describes the result.

	  If a resource has been created on the origin server, the response
	  SHOULD be 201 (Created) and contain an entity which describes the
	  status of the request and refers to the new resource, and a Location
	  header (see section 14.30).

	  Responses to this method are not cacheable, unless the response
	  includes appropriate Cache-Control or Expires header fields. However,
	  the 303 (See Other) response can be used to direct the user agent to
	  retrieve a cacheable resource.

	  POST requests MUST obey the message transmission requirements set out
	  in section 8.2.
	*/
}

/*
bool postRequest::requestURIisvalid()
{
	const ServerNode * server = findServer();

	const LocationRules * location = findServer()->getLocationFromUrl(_requestURI);
}
*/
response deleteRequest::createResponse() {

	response response;
	return response;
}

const char *days[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", NULL };
const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };

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
