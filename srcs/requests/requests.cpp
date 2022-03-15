// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 15:18:45 by pcharton          #+#    #+#             //
//   Updated: 2022/03/15 20:29:08 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/requests.hpp"
#include <cstring>
#include <string.h>

#include <iostream>
getRequest::getRequest() {}
postRequest::postRequest() {}
deleteRequest::deleteRequest() {}


iRequest * iRequest::createRequest(std::string firstLine)
{
	char * method, * requestUri, * httpVersion;

	method = strtok(const_cast<char *>(firstLine.c_str()), " ");
	requestUri = strtok(NULL, " ");
	httpVersion = strtok(NULL, " ");

	if (method && requestUri && httpVersion)
	{
		if (parseMethod(method) && parseHttpVersion(httpVersion))
		{

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

