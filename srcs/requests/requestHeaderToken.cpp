// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestHeaderToken.cpp                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             //
//   Updated: 2022/03/23 20:56:31 by pcharton         ###   ########.fr       //
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

bool isHeaderEnd(const char *input);

class malformedHeader : public std::exception
{
public:
	malformedHeader();
	virtual const char * what () const throw();
};

void checkLineEnd(const std::string &input)
{
	size_t pos = input.find_last_of("\b\n");
	if (!pos || (pos != (input.length() - 1)))
		throw malformedHeader();
}

malformedHeader::malformedHeader() {};
const char * malformedHeader::what() const throw() {
	return ("Header has syntax error");
}

class unfinishedHeader : public std::exception
{
public:
	unfinishedHeader();
	virtual const char * what () const throw();
};

unfinishedHeader::unfinishedHeader() {};
const char * unfinishedHeader::what() const throw() {
	return ("Header is not correctly terminated");
}


requestHeaderToken::requestHeaderToken(const std::pair <std::string, std::string> & rhs) : _token(rhs) {};

std::vector<requestHeaderToken> parseRequestHeader(const char *input)
{
	//this function will change as it expects a full header which might not be possible
	std::vector<requestHeaderToken> requestHeader;
	//change vector for map as there can be only one key field (it seems)

	std::vector<std::string> lineNumber = split_header_to_lines(input);

	for (size_t i = 0; i < lineNumber.size(); i++)
	{
		if (lineNumber[i].find(':') != std::string::npos)
		{
			checkLineEnd(lineNumber[i]);
			lineNumber[i].erase(lineNumber[i].find("\b"));
			requestHeader.push_back(treatLine(lineNumber[i]));
		}
		else
		{
			//check if header end has been reached
			std::string end("\b\n");
			if (i == lineNumber.size() -1 && lineNumber[i] == end)
				break ;
			else
				throw unfinishedHeader();
		}
	}	
	return requestHeader;
}

requestHeaderToken treatLine(std::string line)
{
	size_t sep_index = line.find(":");
	//here we could handle the case of a field unfinished with :
	std::string key(line, 0, sep_index);
	if (sep_index != std::string::npos)
		sep_index += 2;
	line.erase(0, sep_index);
	std::string value(line, 0, line.length());

	return (requestHeaderToken(std::make_pair<std::string, std::string> (key, value)));
}

std::vector<std::string>split_header_to_lines(const char *input)
{
	std::vector<std::string> hold;
	std::string copy(input);

	size_t start = 0;
	size_t end = copy.length();
	
	while (copy.length())
	{
		end = copy.find("\b\n", start);
		if (end != std::string::npos)
			end += 2;
		hold.push_back(std::string(copy, start, end));
		copy.erase(start, end);
	}
	return (hold);
}

bool isHeaderEnd(const char *input)
{
	if (!strcmp("\b\n\b\n", input))
		return (true);
	else
		return (false);
}
