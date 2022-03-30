// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/30 14:06:30 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/requests.hpp"
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

const std::string & iRequest::getRequestURI()
{
	return (_requestURI);
}

response getRequest::createResponse() {
	response response;

	try {
		std::string filePath = createFilePath();
		if (filePath.length())
		{
			response.addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Date", date()));
			response.tryToOpenAndReadFile(filePath);
		}
	}
	catch (std::exception &e){
		//file not found
		std::cout << e.what() << std::endl;
		response.setError404();
//		response.setStatusLine(404);
		return (response);
	}
	response.setStatusLine(200);	
	return response;
}

bool fileExists(std::string file)
{
	if (!access(file.c_str(), F_OK | R_OK))
		return (true);
	else
		return (false);
}

std::string iRequest::createFilePath()
{
	//check each location for the vector
	const ServerNode * test = findServer();
	std::cout << "findServer result : " << test << std::endl;
	if (test)
	{
		const LocationRules * location = test->getLocationFromUrl(getRequestURI());
		std::string filePath;
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
				filePath = (location->root + getRequestURI());
		}
			
		return (filePath);
	}
	return (std::string());
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
		std::cout << port << std::endl;
		std::cout << tmpServerRules.serverName.size() << std::endl;
		for (std::vector<std::string>::iterator names = tmpServerRules.serverName.begin(); names != tmpServerRules.serverName.end(); names++)
		{
			serverName = *names;

			serverName += (":" + to_string(port));
			std::cout << "server name : " << serverName << " host :" << host << std::endl;
			if (serverName == host)
				return (*it);
	}

		std::cout << *it << std::endl;

	}
	return (NULL);
}

response postRequest::createResponse() {

	response response;
	return response;
}

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
