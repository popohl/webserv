// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/30 10:34:56 by pcharton         ###   ########.fr       //
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
		response.setStatusLine(404);
		return (response);
	}
	response.setStatusLine(200);	
	return response;
}

std::string iRequest::createFilePath()
{
	const LocationRules * location = _server->getLocationFromUrl(getRequestURI());
	std::string filePath;
	if (location)
		filePath = (location->root + getRequestURI());
	return (filePath);
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
