// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 11:44:58 by pcharton          #+#    #+#             //
//   Updated: 2022/03/29 14:32:32 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "responses/response.hpp"
#include <sstream>

const std::pair<int, std::string>responseStatus[] = {
	std::make_pair(100, "Continue"),
	std::make_pair(200, "OK"),
	std::make_pair(400, "Bad Request"),
	std::make_pair(404, "Not Found"),
	std::make_pair(405, "Method Not Allowed"),
	std::make_pair(0, ""),
};

fileNotFound::fileNotFound() {}

const char * fileNotFound::what() const throw() {
	return ("File could not be found");
}

response::response() : _headerFields(), _status(), _statusLine(), _header(), _body()
{}

response::response(const response & src) :  _headerFields(src._headerFields),
											_status(src._status),
											_statusLine(src._statusLine),
											_header(src._header), _body(src._body)
{}

response & response::operator = (const response & src) {
	if (this != &src)
	{
		_headerFields = src._headerFields;
		_status = src._status;
		_statusLine = src._statusLine;
		_header = src._header;
		_body = src._body;
	}
	return (*this);
}

response::~response() {}

std::string response::createFormattedResponse()
{
	std::string result;

	result = _statusLine;
//	result += "\r\n";
	if (!_header.length())
		createHeader();
	result += _header;
	result += "\r\n";
	result += _body;

	return (result);
}

void response::addFieldToHeaderMap(std::pair<std::string, std::string>input)
{
	if (_headerFields.find(input.first) == _headerFields.end())
		_headerFields.insert(input);
	else
		_headerFields[input.first] += input.second;
}

void	response::createHeader()
{
	for (std::map<std::string, std::string>::iterator it = _headerFields.begin();
		 it != _headerFields.end();
		 it++)
	{
		_header += it->first;
		_header += ":";
		_header += it->second;
		_header += "\r\n";
	}
}

void response::tryToOpenAndReadFile(std::string filePath)
{
	std::string body;
	char buffer[1048];
	memset(&buffer[0], 0, 1048);

	//improve open and read
	std::ifstream file;
	file.open(filePath.c_str());
	if (file.good())
	{
		std::streamsize bufferSize = 1048;
		try {
			do {
				bufferSize = file.readsome(&buffer[0], bufferSize);
				body += std::string(buffer);
				memset(&buffer[0], 0, 1048);
			} while (bufferSize == 1048);
		}
		catch (std::exception &e) {
			std::cout << "in tryToOpenAndReadFile : " << e.what() << std::endl;
			file.close();
			return;
		}
		file.close();
	}
	else
		throw fileNotFound();
	_body = body;
	if (_body.length())
	{
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Accept", "text/html"));
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Length", to_string(_body.length())));
		_status = 200;
	}
	else
		_status = 404;
}

std::string findStatus(int status)
{
	for (int i = 0; responseStatus[i].first; i++)
	{
		if (responseStatus[i].first == status)
			return responseStatus[i].second;
	}
	return (std::string(""));
}

void response::setStatusLine()
{
	_statusLine = "HTTP/1.1";
	_statusLine += " ";
	_statusLine += to_string(_status);
	_statusLine += " ";
	_statusLine += findStatus(_status);
	_statusLine += "\r\n";
}

std::string to_string(int n)
{
	std::stringstream tmp;

	tmp << n;
	std::string result;

	tmp >> result;
	return (result);
}
/*
std::string to_string(std::streamsize n)
{
	std::stringstream tmp;

	tmp << n;
	std::string result;

	tmp >> result;
	return (result);
}
*/
