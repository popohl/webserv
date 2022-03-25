// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/25 17:49:29 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/requests.hpp"
//#include "requests/requestHeaderToken.hpp"
#include <cstddef>
#include <cstring>
#include <string.h>

getRequest::getRequest() {}
postRequest::postRequest() {}
deleteRequest::deleteRequest() {}

iRequest * iRequest::createRequest(std::string &firstLine) //be able to remove first line from buffer
{
	//rewrite using strings
	char * method, * requestUri, * httpVersion;

	size_t eraseLen = firstLine.find("\r\n");
	method = strtok(const_cast<char *>(firstLine.c_str()), " ");
	requestUri = strtok(NULL, " ");
	httpVersion = strtok(NULL, "\r\n");
	if (method && requestUri && httpVersion)
	{
		if (parseMethod(method) && parseHttpVersion(httpVersion))
		{
			firstLine.erase(0, eraseLen);
			if (!strcmp(method, "GET"))
				return (new getRequest);
			if (!strcmp(method, "POST"))
				return (new postRequest);
			if (!strcmp(method, "DELETE"))
				return (new deleteRequest);
		}
	}
	return (NULL);
}

bool iRequest::receivingisDone()
{
	if (_message._headerFinished && _message._bodyFinished)
		return (true);
	else
		return (false);
}

//std::string 
