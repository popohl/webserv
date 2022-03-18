// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestHeaderToken.cpp                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             //
//   Updated: 2022/03/18 10:59:53 by pcharton         ###   ########.fr       //
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

std::vector<std::string>split_header_to_lines(const char *input);
requestHeaderToken treatLine(std::string line);
	
std::vector<requestHeaderToken> parseRequestHeader(const char *input)
{
	std::vector<requestHeaderToken> requestHeader;
	std::vector<std::string> lineByLineHeader = split_header_to_lines(input);

	for (size_t i = 0; i < lineByLineHeader.size(); i++)
		requestHeader.push_back(treatLine(std::string(lineByLineHeader[i])));
	return requestHeader;
}

requestHeaderToken treatLine(std::string line)
{
	//parse first field
	char * field = strtok(const_cast<char *>(line.c_str()), ":");
	requestHeaderToken token((std::string(field)));

	do {
		field = strtok(NULL, " \n");
		if (field)
			token._input.push_back(std::string(field));
	} while (field);
	return (token);
}

std::vector<std::string>split_header_to_lines(const char *input)
{
	std::vector<std::string> hold;
	std::string copy(input);
	char *line = strtok(const_cast<char *>(copy.c_str()), "\n");
	do {
		hold.push_back(std::string(line));
		line = strtok(NULL, "\n");
	}
	while (line);
	return (hold);
}

/*
  requestHeaderToken::field findField(const char *)
  {
	

}
*/
