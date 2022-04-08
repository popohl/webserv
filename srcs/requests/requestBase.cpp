/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestBase.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             */
/*   Updated: 2022/04/08 11:26:44 by pohl             ###   ########.fr       */
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

requestBase::requestBase() : _headerFinished(false), _bodyFinished(false), _status(), _unfinishedData(),  _header(), _bodySize(0), _bodyExpectedSize(0), _body() {}

bool isbullshit( std::vector<char >& data)
{
	for (std::vector<char>::iterator it = data.begin();
		it != data.end();
		it++)
	{
		if (iscntrl(*it))
			return (true);
	}
	return (false);

}

void requestBase::parseRequest(std::vector<char> &data)
{
	if (isbullshit(data))
	{
		data.clear();
		_headerFinished = true;
		_bodyFinished = true;
		return;
	}

	if (!_headerFinished)
	{
		std::string input(data.begin(), data.end());
		size_t		before = input.length();

		parseHeader(input);

		size_t		after = input.length();
		size_t		diff = before - after;
		data.erase(data.begin(), data.begin() + diff);
	}
	if (_headerFinished && !_bodyFinished && data.size())
	{
		if (_bodyExpectedSize)
			parseBody(data);
		else
			_bodyFinished = true;
	}

}

void requestBase::parseHeader(std::string & input)
{
	std::string line;
	while (input.length())
	{
		line = removeOneHeaderLineFromInput(input);
		if (_unfinishedData.size())
		{
			input.insert(input.begin(), _unfinishedData.begin(), _unfinishedData.end());
			_unfinishedData.clear();
		}
		if (HeaderLineIsCorrectlyFormatted(line))
			this->_header.insert(splitIntoPair(line));
		else if (lineIsHeaderEnd(line))
		{
			_headerFinished = true;
			if (_header.find("Content-Length") != _header.end()
				&& std::strtoul(_header["Content-Length"].c_str(), NULL, 10))
			{
				_bodyExpectedSize = std::strtoul(_header["Content-Length"].c_str(), NULL, 10);
				_body.reserve(_bodyExpectedSize);
			}
			else if ((_header.find("Transfer-Encoding") != _header.end()) && (_chunksList.empty()))
				_bodyExpectedSize = -1;
			else
				_bodyFinished = true;
			break;
		}
		else
			_unfinishedData.insert(_unfinishedData.end(), line.begin(), line.end());
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
		&& (*(line.rbegin()) == '\n')
		&& (*(++(line.rbegin())) == '\r'))
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
void requestBase::parseBody(std::vector<char> & data)
{
	std::map<std::string, std::string>::iterator notFound = _header.end();
	if (_header.find("Transfer-Encoding") != notFound)
	{
		if (_header["Transfer-Encoding"] == "chunked")
			processChunk(data);
	}
	else if	(_header.find("Content-Length") != notFound)
	{
		_body.insert(_body.end(), data.begin(), data.end());
		if (_body.size() >= _bodyExpectedSize)
			_bodyFinished = true;
	}
}

size_t	requestBase::dataContainsCRLF(const std::vector<char> & data)
{
	for (std::vector<char>::const_iterator it = data.begin();
		 it != data.end();
		 it++)
	{
		if ((*it == '\r') && (it != data.end()) && (*(it + 1) == '\n'))
			return true;
	}
	return (false);
}

size_t	requestBase::findCRLFPositionInData(const std::vector<char> & data)
{
	size_t index(0);
	for (std::vector<char>::const_iterator it = data.begin();
		 it != data.end();
		 it++)
	{
		if (!((*it == '\r') && (it != data.end()) && (*(it + 1) == '\n')))
			index++;
		else
			break ;
	}
	return index;
}

void	requestBase::processChunk(std::vector<char> & data)
{
	if (_chunksList.empty() || _chunksList.back())
		_chunksList.push_back(eatChunkSize(data));
	/* all broken, fix it by changing string to vec please
	if (_chunksList.back())
	{
		std::string chunk(line, 0, line.find("\r\n") - 1);
		_body.insert(_body.end(), chunk.begin(), chunk.end());
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
				_bodyFinished = true;
				_bodyExpectedSize = 0;
				_headerFinished = false;
				parseHeader(line);
			}
			else
				_unfinishedField = tmp + line;
		}
	}
	*/
}

size_t requestBase::eatChunkSize(std::vector<char> & data)
{
	std::string chunkSize;
	for (std::vector<char>::const_iterator it = data.begin();
		 isdigit(*it) && it != data.end();
		 it++)
		chunkSize += *it;
	std::stringstream superConverter;
	superConverter << chunkSize;
	size_t result;
	superConverter >> result;
	data.erase(data.begin(), data.begin() + chunkSize.length());
	return (result);
}

std::pair<std::string, std::string>requestBase::splitIntoPair(std::string line)
{
	size_t sep_index = line.find(":");
	std::string key(line, 0, sep_index);
	if (sep_index != std::string::npos)
		sep_index += 1;
	line.erase(0, sep_index);
	size_t whitespace_index = 0;
	while (line[whitespace_index] == ' ')
		whitespace_index++;
	std::string value(line, whitespace_index, line.find("\r\n"));
	return (std::make_pair<std::string, std::string>(std::string(key), std::string(value)));
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

