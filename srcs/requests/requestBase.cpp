// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestBase.cpp                                    :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
/*   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             */
//   Updated: 2022/04/05 12:12:23 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

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

requestBase::requestBase() : _headerFinished(false), _bodyFinished(false), _status(), _unfinishedField(),  _header(), _bodySize(0), _bodyExpectedSize(0), _body() {}

void requestBase::parseRequest(const std::string &line)
{
	std::string copy(line);

	if (!_headerFinished)
		parseHeader(copy);
	if (_headerFinished && !_bodyFinished && copy.length())
	{
		if (_bodyExpectedSize)
			parseBody(copy);
		else
			_bodyFinished = true;
	}
}

void requestBase::parseHeader(std::string & input)
{
	std::string line;
	if (_unfinishedField.length())
	{
		input.insert(0, _unfinishedField);
		_unfinishedField.clear();
	}
	while (input.length())
	{
		line = removeOneHeaderLineFromInput(input);
		if (_unfinishedField.length())
		{
			line.insert(0, _unfinishedField);
			_unfinishedField.clear();
		}
		if (HeaderLineIsCorrectlyFormatted(line))
			this->_header.insert(splitIntoPair(line));
		else if (lineIsHeaderEnd(line))
		{
			_headerFinished = true;
			if (_header.find("Content-Length") != _header.end())
				_bodyExpectedSize = std::strtoul(_header["Content-Length"].c_str(), NULL, 10);
			else if ((_header.find("Transfer-Encoding") != _header.end()) && (_chunksList.empty()))
				_bodyExpectedSize = -1;
			else
				_bodyFinished = true;
			break;
		}
		else
			_unfinishedField = line; 				//it should break automagically
	}
}

std::string	requestBase::removeOneHeaderLineFromInput(std::string & input)
{
	std::string line;
	size_t		position = input.find("\r\n");
	if (position != std::string::npos)
	{
		line = input.substr(0, position + 2);
		input.erase(0, position + 2);
	}
	else
	{
		line = std::string(input);
		input.erase(0, input.length());
	}
	return (line);
}

bool	requestBase::HeaderLineIsCorrectlyFormatted(const std::string & line)
{
	if ((line.find(":") != std::string::npos)
		&& (line.find("\r\n") != std::string::npos)
		&& (*(--(line.end())) == '\n')
		&& (*(--(--(line.end()))) == '\r'))
		return (true);
	else
		return (false);
}

bool	requestBase::lineIsHeaderEnd(const std::string & line)
{
	if ((line == "\r\n") && (line.length() == 2))
		return (true);
	else
		return (false);
}

//it should only get the body string
void requestBase::parseBody(std::string &line)
{
	std::map<std::string, std::string>::iterator notFound = _header.end();
	if (_header.find("Transfer-Encoding") != notFound)
	{
		if (_header["Transfer-Encoding"] == "chunked")
			//parse chunked body
			processChunk(line);

	}
	else if	(_header.find("Content-Length") != notFound)
	{
		size_t sizeDifference(_bodyExpectedSize - _body.length());
		_body.insert(_body.length(), line, 0, sizeDifference);
		if (_body.length() >= _bodyExpectedSize)
			_bodyFinished = true;
	}
}

void	requestBase::processChunk(std::string & line)
{
	std::cout << "in process chgunck line size before eat " << line.length() << std::endl;
	if (_chunksList.empty() || _chunksList.back())
		_chunksList.push_back(eatChunkSize(line));
	std::cout << "in process chgunck line size after eat " << line.length() << std::endl;
	if (_chunksList.back())
	{
		_body += std::string(line, 0, line.find("\r\n") - 1);
		line.erase(0, line.find("\r\n") + 2);
	}
	else
	{
		if (line.length())
		{
			if (_unfinishedField.length())
			{
				line.insert(0, _unfinishedField);
				_unfinishedField.clear();
			}
			std::string tmp = removeOneHeaderLineFromInput(line);
			if (lineIsHeaderEnd(tmp))
			{
				std::cout << "found last chunck CRLF, line length is " <<line.length() << std::endl;
				_bodyFinished = true;
				_bodyExpectedSize = 0;
				_headerFinished = false;
				if (line[0] == '\r')
					std::cout << "CR FOUND IN LINE" << std::endl;
				parseHeader(line);
				std::cout << "after parse Header, line length is "  <<line.length() << std::endl;
			}
			else
			{
				_unfinishedField = tmp + line;
			}

		}

	}
}

size_t requestBase::eatChunkSize(std::string & line)
{
	std::string chunkSize(line, 0, line.find("\r\n") - 1);
	std::stringstream superConverter;
	std::cout << "line is " + chunkSize << std::endl;
	superConverter << chunkSize;
	size_t result;
	superConverter >> result;
	std::cout << "chunk size is " << result  << std::endl;
	line.erase(0, chunkSize.length() + 2);
	return (result);
}

std::pair<std::string, std::string>requestBase::splitIntoPair(std::string line)
{
	size_t sep_index = line.find(":");
	//here we could handle the case of a field unfinished with :
	std::string key(line, 0, sep_index);
	if (sep_index != std::string::npos)
		sep_index += 1;
	line.erase(0, sep_index);
	size_t whitespace_index = 0;
	while (line[whitespace_index] == ' ')
		whitespace_index++;
	std::string value(line, whitespace_index, line.find("\r\n") - 1);
	return (std::make_pair<std::string, std::string>(std::string(key), std::string(value)));
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

