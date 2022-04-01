// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 11:44:58 by pcharton          #+#    #+#             //
//   Updated: 2022/04/01 11:49:28 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "responses/response.hpp"
#include <sstream>

const std::pair<int, std::string>responseStatus[] = {
	std::make_pair(100, "Continue"),
	std::make_pair(101, "Switching Protocols"),
	std::make_pair(200, "OK"),
	std::make_pair(201,"Created"),
	std::make_pair(202,"Accepted"),
	std::make_pair(203, "Non-Authoritative Information"),
	std::make_pair(204, "No Content"),
	std::make_pair(205, "Reset Content"),
	std::make_pair(206, "Partial Content"),
	std::make_pair(300, "Multiple Choices"),
	std::make_pair(301, "Moved Permanently"),
	std::make_pair(302, "Found"),
	std::make_pair(303, "See Other"),
	std::make_pair(304, "Not Modified"),
	std::make_pair(305, "Use Proxy"),
	std::make_pair(307, "Temporary Redirect"),
	std::make_pair(400, "Bad Request"),
	std::make_pair(401, "Unauthorized"),
	std::make_pair(402, "Payment Required"),
	std::make_pair(403, "Forbidden"),
	std::make_pair(404, "Not Found"),
	std::make_pair(405, "Method Not Allowed"),
	std::make_pair(406, "Not Acceptable"),
	std::make_pair(407, "Proxy Authentication Required"),
	std::make_pair(408, "Request Time-out"),
	std::make_pair(409, "Conflict"),
	std::make_pair(410, "Gone"),
	std::make_pair(411, "Length Required"),
	std::make_pair(412, "Precondition Failed"),
	std::make_pair(413, "Request Entity Too Large"),
	std::make_pair(414, "Request-URI Too Large"),
	std::make_pair(415, "Unsupported Media Type"),
	std::make_pair(416, "Requested range not satisfiable"),
	std::make_pair(417, "Expectation Failed"),
	std::make_pair(500, "Internal Server Error"),
	std::make_pair(501, "Not Implemented"),
	std::make_pair(502, "Bad Gateway"),
	std::make_pair(503, "Service Unavailable"),
	std::make_pair(504, "Gateway Time-out"),
	std::make_pair(505, "HTTP Version not supported"),
	std::make_pair(0, ""),
};

fileNotFound::fileNotFound() {}

const char * fileNotFound::what() const throw() {
	return ("File could not be found");
}

fileCouldNotBeOpen::fileCouldNotBeOpen() {}

const char * fileCouldNotBeOpen::what() const throw() {
	return ("File could not be open");
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
		throw fileCouldNotBeOpen();
	_body = body;
	if (_body.length())
	{
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Accept", "text/html"));
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Length", to_string(_body.length())));
		setStatusLine(200);
	}
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

void response::setStatusLine(int status)
{
	_statusLine = "HTTP/1.1";
	_statusLine += " ";
	_statusLine += to_string(status);
	_statusLine += " ";
	_statusLine += findStatus(status);
	_statusLine += "\r\n";
}

void response::setErrorMessage(int errorStatus)
{	
	setStatusLine(errorStatus);
	addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Accept", "text/plain"));
	_body = formatErrorMessage(errorStatus);
	addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Length", to_string(_body.length())));
}

std::string to_string(int n)
{
	std::stringstream tmp;

	tmp << n;
	std::string result;

	tmp >> result;
	return (result);
}

std::string formatErrorMessage(int errorStatus)
{
	std::string result;

	for (int i = 0; responseStatus[i].first; i++)
	{
		if (responseStatus[i].first == errorStatus)
		{
			result = "Error ";
			result += to_string(errorStatus);
			result += "\n";
			result += responseStatus[i].second;
			return (result);
		}
	}
	result = "Very bad Error, you should never see this message\nIt means that no responseStatus were found.";
	return (result);
}
