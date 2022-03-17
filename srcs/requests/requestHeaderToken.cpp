// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestHeaderToken.cpp                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             //
//   Updated: 2022/03/17 18:05:58 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/requestHeaderToken.hpp"
#include <string.h>
#include <cstring>
#include <string>
#include <iostream>

const char *field_table[] = {
	"Accept",
	"Accept-Charset",
	"Accept-Encoding",
	"Accept-Language",
	"Accept-Ranges",
	"Age",
	"Allow",
	"Authorization",
	"Cache-Control",
	"Connection",
	"Content-Encoding",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-MD5",
	"Content-Range",
	"Content-Type",
	"Date",
	"Clockless Origin Server Operation",
	"ETag",
	"Expect",
	"Expires",
	"From",
	"Host",
	"If-Match",
	"If-Modified-Since",
	"If-None-Match",
	"If-Range",
	"If-Unmodified-Since",
	"Last-Modified",
	"Location",
	"Max-Forwards",
	"Pragma",
	"Proxy-Authenticate",
	"Proxy-Authorization",
	"Range", //Range = "Range" ":" ranges-specifier
	"Referer",
	"Retry-After",
	"Server",
	"TE",
	"Trailer",
	"Transfer-Encoding",
	"Upgrade",
	"User-Agent",
	"Vary",
	"Via",
	"Warning",
	"WWW-Authenticate",
	NULL };

requestHeaderToken::requestHeaderToken() : _type(), _input() {};
requestHeaderToken::requestHeaderToken(std::string type) : _type(type), _input() {};

std::vector<requestHeaderToken> parseRequestHeader(const char *input)
{
	std::vector<requestHeaderToken> requestHeader;

	char *line = NULL;
	line = strtok(const_cast<char *>(input), "\n");
	requestHeader.push_back(treatLine(line));
	while (line)
	{
		line = strtok(NULL, "\n");
		requestHeader.push_back(treatLine(line));
	}
	for (size_t i = 0; i < requestHeader.size(); i++)
		std::cout << requestHeader[i]._type << std::endl;
	return requestHeader;
}

requestHeaderToken treatLine(const char *line)
{
	char * field = strtok(const_cast<char *>(line), ":");
	requestHeaderToken token((std::string(field)));
	do {
		field = strtok(NULL, " \n");
		token._input.push_back(std::string(field));
	} while (field);
	return (token);
}
/*
requestHeaderToken::field findField(const char *)
{
	

}
*/
