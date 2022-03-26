// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/26 17:59:37 by pcharton         ###   ########.fr       //
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
	//rewrite using strings
	iRequest * result = NULL;
	char * method = NULL;
	char * requestUri = NULL;
	char * httpVersion = NULL;

	size_t eraseLen = input.find("\r\n");
	std::string firstLine(input, 0, eraseLen);
	if (eraseLen != std::string::npos)
	{
		method = strtok(const_cast<char *>(firstLine.c_str()), " ");
		requestUri = strtok(NULL, " ");
		httpVersion = strtok(NULL, "\r\n");
	}

	if (method && requestUri && httpVersion)
	{
		if (parseMethod(method) && parseHttpVersion(httpVersion))
		{
			if (!strcmp(method, "GET"))
				result = new getRequest;
			if (!strcmp(method, "POST"))
				result = new postRequest;
			if (!strcmp(method, "DELETE"))
				result = new deleteRequest;
			input.erase(0, eraseLen + 2);
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

