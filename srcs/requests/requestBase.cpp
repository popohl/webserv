/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestBase.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 16:53:04 by pcharton          #+#    #+#             */
//   Updated: 2022/04/09 15:20:11 by pcharton         ###   ########.fr       //
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

void	removeNonPrintableCharacters( std::vector<char >& data)
{
	std::vector<char>::iterator it = data.begin();
	while (it != data.end())
	{
		size_t toErase = 0;
		if (!isprint(*it) && !isspace(*it))
		{
			size_t position = it - data.begin();
			while (!isprint(*(it + toErase))
				   && !isspace(*(it + toErase))
				   && (it + toErase) != data.end())
				toErase++;
			data.erase(it, it + toErase);
			if (position <  data.size())
				it = data.begin() + position;
			else
				it = data.end();
		}
		else
			it++;
	}
}

void requestBase::parseRequest(std::vector<char> &data)
{

	if (!_headerFinished && data.size())
	{
		removeNonPrintableCharacters(data);
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
		//does not work if file is bigger than recv buffer
		if (_header["Transfer-Encoding"] == "chunked")
		{
			while (!data.empty()
				&& !_bodyFinished)
				chunkedTransferEncodingRoutine(data);
			if (_chunksList.size() && _chunksList.back().isLastChunk()
				&& _chunksList.back()._sizeDelimiterFound)
				transformChunkListIntoData();
		}
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

void	requestBase::chunkedTransferEncodingRoutine(std::vector<char> & data)
{
	if (_chunksList.empty())
	{
		_chunksList.push_back(chunk());
		chunk & current = _chunksList.back();
		current.eatChunkSize(data);
		current.chunkProcedure(data);
	}
	else if (!(_chunksList.back().isLastChunk()))
	{
		if (!_chunksList.back()._chunkIsDone)
		{
			chunk & current = _chunksList.back();
			current.chunkProcedure(data);
		}
		else
		{
			_chunksList.push_back(chunk());
			chunk & current = _chunksList.back();
			current.eatChunkSize(data);
			current.chunkProcedure(data);
		}
	}
	else
	{
		if (_chunksList.back()._sizeDelimiterFound)
		{
			_bodyFinished = true;
			_bodyExpectedSize = 0;
			_headerFinished = false;
			std::string header(data.begin(), data.end());
			parseHeader(header);	
		}
		else
		{
			chunk & current = _chunksList.back();
			current.eatCRLF(data);
		}
	}
}

void	requestBase::transformChunkListIntoData()
{
	size_t totalSize = 0;
	for (std::deque<chunk>::iterator it = _chunksList.begin();
		it != _chunksList.end(); it++)
		totalSize += it->_chunkSize;
	_body.reserve(_bodySize + totalSize);
	for (std::deque<chunk>::iterator it = _chunksList.begin();
		it != _chunksList.end() && it->_chunkSize; it = _chunksList.begin())
		{
			_body.insert(_body.end(), it->_chunkData.begin(), it->_chunkData.end());
			_chunksList.erase(_chunksList.begin());
		}
}

void	chunk::chunkProcedure(std::vector<char> & data)
{
	if (!_sizeDelimiterFound && eatCRLF(data))
		_sizeDelimiterFound = true;
	if (_sizeDelimiterFound && _chunkSize)
		tryToEatChunkData(data);
	if (_sizeDelimiterFound && _chunkDelimiterFound && _chunkHasBeenProcessed)
		_chunkIsDone = true;
}

bool	chunk::eatCRLF(std::vector<char> & data)
{
	if ((data.size() >= 2) && (data[0] == '\r') && (data[1] == '\n'))
	{
		data.erase(data.begin(), data.begin() + 2);
		return (true);
	}
	else
		return (false);
}	

void	chunk::tryToEatChunkData(std::vector<char> & data)
{
	if (_chunkData.capacity() < _chunkSize)
		_chunkData.reserve(_chunkSize);
	size_t	bytesToProcess = _chunkSize - _chunkData.size();
	std::vector<char>::iterator ite;
	if (data.size() >= bytesToProcess)
		ite = data.begin() + bytesToProcess;
	else
		ite = data.end();
	_chunkData.insert(_chunkData.end(), data.begin(), ite);
	data.erase(data.begin(), ite);
	if (_chunkSize == _chunkData.size())
		_chunkHasBeenProcessed = true;
	if (_chunkHasBeenProcessed && eatCRLF(data))
		_chunkDelimiterFound = true;
}

void chunk::eatChunkSize(std::vector<char> & data)
{
	std::string chunkSize;
	for (std::vector<char>::const_iterator it = data.begin();
		 isxdigit(*it) && it != data.end();
		 it++)
		chunkSize += *it;
	
	std::stringstream superConverter;
	std::cout << "chunkSize is " << chunkSize << "of size " << chunkSize.size() << std::endl;
	superConverter << std::hex << chunkSize;
	superConverter >> _chunkSize;
	std::cout << "converted size is " << _chunkSize << std::endl;
	data.erase(data.begin(), data.begin() + chunkSize.length());
	if (eatCRLF(data))
		_sizeDelimiterFound = true;
	if (!_chunkSize && _sizeDelimiterFound)
		_chunkIsDone = true;
}

bool	chunk::isLastChunk()
{
	if (!_chunkSize)
		return (true);
	else
		return (false);
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
	if (!_header.empty() && (_header["Host"].size()))
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
