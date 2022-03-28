// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/28 11:16:41 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/requests.hpp"
//#include "requests/requestHeaderToken.hpp"
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

iRequest * iRequest::createRequest(std::string &input) //be able to remove first line from buffer
{
	iRequest * result = NULL;
	std::string method, requestUri, httpVersion;

	//isolate first line and three string tokens
	size_t eraseLen = input.find("\r\n");
	std::string firstLine(input, 0, eraseLen);
	if (eraseLen != std::string::npos)
	{
		std::string requestLine(firstLine, 0, eraseLen);
		//Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
		
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

std::string getRequest::createResponse() {
	std::string response;

	response += "HTTP/1.1 200 Ok\r\n";
	if (_message._status != 500 && _message._status != 503)
		response += date();
	response += "Accept: /text/plain\r\n";
	response += "Content-length: 5\r\n"; //replace it with the length of the body to send
	response += "\r\n";
	//body
	response += "Hello"; //body to send
	return response;
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

