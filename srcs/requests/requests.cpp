// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/26 13:00:38 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/requests.hpp"
//#include "requests/requestHeaderToken.hpp"
#include <cstddef>
#include <cstring>
#include <string.h>
#include <iostream>

getRequest::getRequest() {}
postRequest::postRequest() {}
deleteRequest::deleteRequest() {}

iRequest * iRequest::createRequest(std::string &firstLine) //be able to remove first line from buffer
{
	//rewrite using strings
	iRequest * result = NULL;
	char * method, * requestUri, * httpVersion;

	size_t eraseLen = firstLine.find("\r\n");

	//std::cout << "value of eraseLen : " <<eraseLen << std::endl;

	method = strtok(const_cast<char *>(firstLine.c_str()), " ");
	requestUri = strtok(NULL, " ");
	httpVersion = strtok(NULL, "\r\n");
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
			firstLine.erase(0, eraseLen + 2);
			std::cout << "content after erase : size : " << firstLine.length() << "|" << firstLine << "|" << std::endl;
			result->_message.parseRequest(firstLine);
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

	response += "HTTP/1.1 200 OK\r\n";
	response += "\r\n";
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
