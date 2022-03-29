// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/29 10:31:33 by pcharton         ###   ########.fr       //
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

getRequest::getRequest() {}
postRequest::postRequest() {}
deleteRequest::deleteRequest() {}

iRequest * iRequest::createRequest(std::string &input, ServerNode * server) //be able to remove first line from buffer
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
		requestUri = eatWord(requestLine);
		httpVersion = eatWord(requestLine);
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
			result->_server = server;
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

std::string getRequest::createResponse() {
	std::string response;

	std::cout << _server << std::endl;
	response += "HTTP/1.1 200 Ok\r\n";
	if (_message._status != 500 && _message._status != 503)
		response += date();
	response += "Accept: /text/html\r\n";

	response += "Content-length: 108\r\n"; //replace it with the length of the body to send
	response += "\r\n";
	//body


	response += createResponseBody();

	return response;
}

std::string getRequest::createResponseBody()
{
	std::string body;

	char buffer[1048];
	memset(&buffer[0], 0, 1048);

	const LocationRules * location = _server->getLocationFromUrl(getRequestURI());
	if (location)
	{
		std::string filePath(location->root + getRequestURI());
		std::ifstream file;
		file.open(filePath.c_str());
		if (file.good())
		{
			file.readsome(&buffer[0], 1048);
			body += std::string(buffer);
		}
	}
	return (body);
	
}

std::string postRequest::createResponse() {

	std::string response;
	return response;
}

std::string deleteRequest::createResponse() {

	std::string response;
	return response;
}

const char *days[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", NULL };
const char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL };

std::string date()
{
	time_t now = time(0);
	tm * gmt = gmtime(&now);	

	std::string result("Date:");
	std::stringstream tmp;
	tmp << " " << days[gmt->tm_wday];
	tmp << ", " << gmt->tm_mday;
	tmp << " " << months[gmt->tm_mon];
	tmp << " " << 1900 + gmt->tm_year;
	tmp << " " << gmt->tm_hour;
	tmp << ":" << gmt->tm_min;
	tmp << ":" << gmt->tm_sec;

	result += tmp.str();
	result += " GMT\r\n";
	return (result);
}

