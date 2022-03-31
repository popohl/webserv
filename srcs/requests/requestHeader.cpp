/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestHeader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             */
//   Updated: 2022/03/31 14:56:38 by pcharton         ###   ########.fr       //
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

requestBase::requestBase() : _headerFinished(false), _bodyFinished(false), _header(), _bodySize(0), _bodyExpectedSize(0), _body() {}

void requestBase::parseRequest(const std::string &line)
{
	std::string copy(line);

	if (!_headerFinished)
		parseHeader(copy);
	if (_headerFinished && !_bodyFinished)
	{
		if (_bodyExpectedSize)
			parseBody(copy);
		else
			_bodyFinished = true;
	}
}

void	requestBase::parseHeader(std::string &line) {

	std::list<std::string> lineNumber = split_header_to_lines(line);

	for (std::list<std::string>::iterator it = lineNumber.begin(); it != lineNumber.end(); it = lineNumber.begin())
	{
		if (it->find(':') != std::string::npos)
		{
			//remove \r\n
			checkLineEnd(*it);
			it->erase(it->find("\r"));
			//get pair
			std::pair<std::string, std::string>parsedPair = splitIntoPair(*it);
			if (parsedPair.first == "Content-Length") //dirty update of body expected size
				_bodyExpectedSize = atoi(parsedPair.second.c_str());
			if (_header.find(parsedPair.first) == _header.end())
				_header.insert(parsedPair);
			else
				throw fieldAlreadyExists();  //either throw an exception or concatenate the result to the pair value
			line.erase(0, it->length() + 2);
			lineNumber.erase(lineNumber.begin());
		}
		else
		{
			//check if header end has been reached
			std::string end("\r\n");
			if (*it == end && it->length() == end.length())
			{
				_headerFinished = true;
				line.erase(0, 2);
				break ;
			}
			else
				throw unfinishedHeader();
		}
	}
}

//it should only get the body string
void requestBase::parseBody(const std::string &line)
{
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
		size_t sizeDifference(_bodyExpectedSize - _body.length());
		_body.insert(_body.length(), line, 0, sizeDifference);
		if (_body.length() >= _bodyExpectedSize)
			_bodyFinished = true;
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

std::list<std::string>split_header_to_lines(const std::string & input)
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
	if (!_bodySize)
		return (result);
	else if (result > _bodySize)
		return (result - _bodySize);
	else
		return (0);
}

