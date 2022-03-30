/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestHeader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             */
//   Updated: 2022/03/30 14:55:25 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "requests/requestBase.hpp"
#include <string.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <utility>

malformedHeader::malformedHeader() {};
const char * malformedHeader::what() const throw() {
	return ("Header has syntax error");
}

unfinishedHeader::unfinishedHeader() {};
const char * unfinishedHeader::what() const throw() {
	return ("Header is not correctly terminated");
}

fieldAlreadyExists::fieldAlreadyExists() {};
const char * fieldAlreadyExists::what() const throw() {
	return ("Field has already been parsed previously");
}

void checkLineEnd(const std::string &input)
{
	size_t pos = input.find_last_of("\r\n");
	if (!pos || (pos != (input.length() - 1)))
		throw malformedHeader();
}

requestBase::requestBase() : _headerFinished(false), _bodyFinished(false), _header(), _bodySize(0), _body() {}

void requestBase::parseRequest(const std::string &line)
{
	size_t headerSize = 0; //might not work for 
	if (!_headerFinished)
		headerSize = parseHeader(line);
	else
		headerSize = findBodyLength();
//	std::cout << line.length() << "|" << headerSize << std::endl;
	if (!_bodyFinished && (headerSize < line.length()))
	{
		std::string body(line, headerSize, line.length());
		parseBody(body);
	}
	else
		_bodyFinished = true; //are you sure ?
//	std::cout << line.length() << "|" << headerSize  << "|" << _body.length()<< std::endl;
}

size_t requestBase::parseHeader(const std::string &line) {
	size_t headerSize = 0;
	std::list<std::string> lineNumber = split_header_to_lines(line, headerSize);

	for (std::list<std::string>::iterator it = lineNumber.begin(); it != lineNumber.end(); it = lineNumber.begin())
	{
		if (it->find(':') != std::string::npos)
		{
			//remove \r\n
			checkLineEnd(*it);
			it->erase(it->find("\r"));
			//get pair
			std::pair<std::string, std::string>parsedPair = splitIntoPair(*it);
			if (_header.find(parsedPair.first) == _header.end())
				_header.insert(parsedPair);
			else
				throw fieldAlreadyExists();  //either throw an exception or concatenate the result to the pair value
			lineNumber.erase(lineNumber.begin());
		}
		else
		{
			//check if header end has been reached
			std::string end("\r\n");
			if (*it == end && it->length() == end.length())
			{
				_headerFinished = true;
				headerSize++;
				break ;
			}
			else
				throw unfinishedHeader();
		}
	}
	return (headerSize);
}

//it should only get the body string
void requestBase::parseBody(const std::string &line)
{
	std::string copy(line);

	std::map<std::string, std::string>::iterator notFound = _header.end();
	if (_header.find("Transfert-Encoding") != notFound)
	{
		if (_header["Transfert-Encoding"] == "chuncked")
		{
			//parse chunked body
		}
	}
	else if	(_header.find("Content-Length") != notFound)
	{
		if (!_bodySize)
			_bodySize = atoi(_header["Content-Length"].c_str());//, NULL, 10);
		if (_body.length() < _bodySize)
			_body += copy;
	}
}

std::pair<std::string, std::string>requestBase::splitIntoPair(std::string line)
{
	size_t sep_index = line.find(":");
	//here we could handle the case of a field unfinished with :
	std::string key(line, 0, sep_index);
	if (sep_index != std::string::npos)
		sep_index += 1;
	line.erase(0, sep_index);
	std::string value(line, 0, line.length());

	return (std::make_pair<std::string, std::string>(key, value));
}

std::list<std::string>split_header_to_lines(const std::string & input, size_t &headerSize)
{
	std::list<std::string> hold;
	std::string copy(input);

	size_t start = 0;
	size_t end = copy.length();

	while (copy.length())
	{
		end = copy.find("\r\n", start);
		if (end != std::string::npos)
			end += 2;
		hold.push_back(std::string(copy, start, end));
		copy.erase(start, end);

		headerSize += end;
	}
	return (hold);
}

bool isHeaderEnd(const char *input)
{
	if (!strcmp("\r\n\r\n", input))
		return (true);
	else
		return (false);
}

bool requestBase::containsHostField(void)
{
	if (_header.find("Host") != _header.end())
		return (true);
	else
		return (false);
}

void requestBase::updateResponseStatus(void)
{
	std::map<std::string, std::string>::iterator notFound = _header.end();
	if (_header.find("Host") == notFound)
		_status = 400;
}
//NB server should return 400 Bad Request for a request missing host field in http 1.1 protocol

size_t requestBase::findBodyLength(void)
{
	size_t result = 0;
	std::map<std::string, std::string>::iterator notFound = _header.end();
	if (_header.find("Content-Length") != notFound)
	{
		std::stringstream nb(_header["Content-Length"]);
		nb >> result;
	}
	return (result);
}

