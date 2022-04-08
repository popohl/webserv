// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
/*   response.cpp                                       :+:      :+:    :+:   */
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 11:44:58 by pcharton          #+#    #+#             //
/*   Updated: 2022/04/08 11:26:44 by pohl             ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#include "responses/httpExceptions.hpp"
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

const std::pair<std::string, std::string>documentType[] = {
	std::pair<std::string, std::string>("html", "text/html"),
	std::pair<std::string, std::string>("htm","text/html"),
	std::pair<std::string, std::string>("shtml","text/html"),
	std::pair<std::string, std::string>("css","text/css"),
	std::pair<std::string, std::string>("xml","text/xml"),
	std::pair<std::string, std::string>("gif","image/gif"),
	std::pair<std::string, std::string>("jpeg","image/gif"),
	std::pair<std::string, std::string>("jpg","image/jpeg"),
	std::pair<std::string, std::string>("js","application/javascript"),
	std::pair<std::string, std::string>("atom","application/atom+xml"),
	std::pair<std::string, std::string>("rss","application/rss+xml"),
	std::pair<std::string, std::string>("mml","text/mathml"),
	std::pair<std::string, std::string>("txt","text/plain"),
	std::pair<std::string, std::string>("jad","text/vnd.sun.j2me.app-descriptor"),
	std::pair<std::string, std::string>("wml","text/vnd.wap.wml"),
	std::pair<std::string, std::string>("htc","text/x-component"),
	std::pair<std::string, std::string>("png","image/png"),
	std::pair<std::string, std::string>("tif","image/png"),
	std::pair<std::string, std::string>("tiff","image/tiff"),
	std::pair<std::string, std::string>("wbmp","image/vnd.wap.wbmp"),
	std::pair<std::string, std::string>("ico","image/x-icon"),
	std::pair<std::string, std::string>("jng","image/x-jng"),
	std::pair<std::string, std::string>("bmp","image/x-ms-bmp"),
	std::pair<std::string, std::string>("svg","image/x-ms-bmp"),
	std::pair<std::string, std::string>("svgz","image/svg+xml"),
	std::pair<std::string, std::string>("webp","image/webp"),
	std::pair<std::string, std::string>("woff","application/font-woff"),
	std::pair<std::string, std::string>("jar","application/font-woff"),
	std::pair<std::string, std::string>("war","application/font-woff"),
	std::pair<std::string, std::string>("ear","application/java-archive"),
	std::pair<std::string, std::string>("json","application/json"),
	std::pair<std::string, std::string>("hqx","application/mac-binhex40"),
	std::pair<std::string, std::string>("doc","application/msword"),
	std::pair<std::string, std::string>("pdf","application/pdf"),
	std::pair<std::string, std::string>("ps","application/pdf"),
	std::pair<std::string, std::string>("eps", "application/pdf"),
	std::pair<std::string, std::string>("ai", "application/postscript"),
	std::pair<std::string, std::string>("rtf", "application/rtf"),
	std::pair<std::string, std::string>("m3u8", "application/vnd.apple.mpegurl"),
	std::pair<std::string, std::string>("xls", "application/vnd.ms-excel"),
	std::pair<std::string, std::string>("eot", "application/vnd.ms-fontobject"),
	std::pair<std::string, std::string>("ppt", "application/vnd.ms-powerpoint"),
	std::pair<std::string, std::string>("wmlc", "application/vnd.wap.wmlc"),
	std::pair<std::string, std::string>("kml", "application/vnd.google-earth.kml+xml"),
	std::pair<std::string, std::string>("kmz", "application/vnd.google-earth.kmz"),
	std::pair<std::string, std::string>("7z", "application/x-7z-compressed"),
	std::pair<std::string, std::string>("cco", "application/x-cocoa"),
	std::pair<std::string, std::string>("jardiff", "application/x-java-archive-diff"),
	std::pair<std::string, std::string>("jnlp", "application/x-java-jnlp-file"),
	std::pair<std::string, std::string>("run", "application/x-makeself"),
	std::pair<std::string, std::string>("pl", "application/x-makeself"),
	std::pair<std::string, std::string>("pm", "application/x-perl"),
	std::pair<std::string, std::string>("prc", "application/x-perl"),
	std::pair<std::string, std::string>("pdb", "application/x-pilot"),
	std::pair<std::string, std::string>("rar", "application/x-rar-compressed"),
	std::pair<std::string, std::string>("rpm", "application/x-redhat-package-manager"),
	std::pair<std::string, std::string>("sea", "application/x-sea"),
	std::pair<std::string, std::string>("swf", "application/x-shockwave-flash"),
	std::pair<std::string, std::string>("sit", "application/x-stuffit"),
	std::pair<std::string, std::string>("tcl", "application/x-stuffit"),
	std::pair<std::string, std::string>("tk", "application/x-tcl"),
	std::pair<std::string, std::string>("der", "application/x-tcl"),
	std::pair<std::string, std::string>("pem", "application/x-tcl"),
	std::pair<std::string, std::string>("crt", "application/x-x509-ca-cert"),
	std::pair<std::string, std::string>("xpi", "application/x-xpinstall"),
	std::pair<std::string, std::string>("xhtml", "application/xhtml+xml"),
	std::pair<std::string, std::string>("xspf", "application/xspf+xml"),
	std::pair<std::string, std::string>("zip", "application/zip"),
	std::pair<std::string, std::string>("bin", "application/zip"),
	std::pair<std::string, std::string>("exe", "application/zip"),
	std::pair<std::string, std::string>("dll", "application/octet-stream"),
	std::pair<std::string, std::string>("deb", "application/octet-stream"),
	std::pair<std::string, std::string>("dmg", "application/octet-stream"),
	std::pair<std::string, std::string>("iso", "application/octet-stream"),
	std::pair<std::string, std::string>("img", "application/octet-stream"),
	std::pair<std::string, std::string>("msi", "application/octet-stream"),
	std::pair<std::string, std::string>("msp", "application/octet-stream"),
	std::pair<std::string, std::string>("msm", "application/octet-stream"),
	std::pair<std::string, std::string>("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
	std::pair<std::string, std::string>("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"),
	std::pair<std::string, std::string>("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"),
	std::pair<std::string, std::string>("mid", "application/vnd.openxmlformats-officedocument.presentationml.presentation"),
	std::pair<std::string, std::string>("midi", "application/vnd.openxmlformats-officedocument.presentationml.presentation"),
	std::pair<std::string, std::string>("kar", "audio/midi"),
	std::pair<std::string, std::string>("mp3", "audio/mpeg"),
	std::pair<std::string, std::string>("ogg", "audio/ogg"),
	std::pair<std::string, std::string>("m4a", "audio/x-m4a"),
	std::pair<std::string, std::string>("ra", "audio/x-realaudio"),
	std::pair<std::string, std::string>("3gpp", "audio/x-realaudio"),
	std::pair<std::string, std::string>("3gp", "video/3gpp"),
	std::pair<std::string, std::string>("ts", "video/mp2t"),
	std::pair<std::string, std::string>("mp4", "video/mp4"),
	std::pair<std::string, std::string>("mpeg", "video/mp4"),
	std::pair<std::string, std::string>("mpg", "video/mpeg"),
	std::pair<std::string, std::string>("mov", "video/quicktime"),
	std::pair<std::string, std::string>("webm", "video/webm"),
	std::pair<std::string, std::string>("flv", "video/x-flv"),
	std::pair<std::string, std::string>("m4v", "video/x-m4v"),
	std::pair<std::string, std::string>("mng", "video/x-mng"),
	std::pair<std::string, std::string>("asx", "video/x-mng"),
	std::pair<std::string, std::string>("asf", "video/x-ms-asf"),
	std::pair<std::string, std::string>("wmv", "video/x-ms-wmv"),
	std::pair<std::string, std::string>("avi", "video/x-msvideo"),
	std::pair<std::string, std::string>("END", ""),
};

response::response() : _headerFields(), _status(), _statusLine(), _header(), _body(), _file()
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

response::~response()
{
	if (_file)
		_file.close();
}

void response::printHeader() { std::cout << _header << std::endl;}
void response::printStatus() { std::cout << "status line : " +_statusLine << std::endl; }


std::vector<char> response::createFormattedResponse()
{
	std::vector<char>raw;
	createHeader();
	size_t size = _header.length();
	if (_body.length())
		size += _body.length();
	else
		size += getResponseFileSize();
	raw.reserve(size);
	raw.insert(raw.begin(), _header.begin(), _header.end());
	if (_body.length())
		raw.insert(raw.end(), _body.begin(), _body.end());
	else
		readWholeFile(raw);
	return (raw);
}

void response::addFieldToHeaderMap(std::pair<std::string, std::string>input)
{
	if (_headerFields.find(input.first) == _headerFields.end())
		_headerFields.insert(input);
	else
		_headerFields[input.first] += (", " + input.second);
}

void response::replaceFieldToHeaderMap(std::pair<std::string, std::string>input)
{
	if (_headerFields.find(input.first) == _headerFields.end())
		_headerFields.insert(input);
	else
		_headerFields[input.first] = (input.second);

}

void	response::createHeader()
{
	_header = _statusLine;
	for (std::map<std::string, std::string>::iterator it = _headerFields.begin();
		 it != _headerFields.end();
		 it++)
	{
		_header += it->first;
		_header += ":";
		_header += it->second;
		_header += "\r\n";
	}
	_header += "\r\n";
}

void response::tryToOpenFile(std::string filePath)
{
	if (!fileExists(filePath))
		throw httpError(404, "Requested file not found");
	_file.open(filePath.c_str(), std::ios::in | std::ios::binary);
	if (_file.good())
	{
		if (_headerFields.count("Content-Type") == 0)
			addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Type", findContentType(filePath)));
		if (_headerFields.count("Content-Length") == 0)
			addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Length", to_string(getResponseFileSize())));
	}
	else
		throw httpError(403, "Requested file could not be opened");
}

size_t response::getResponseFileSize()
{
	if (_file.good())
	{
		size_t currentPos = _file.tellg();
		size_t fileSize = 0;

		_file.seekg(0, _file.end);
		fileSize = _file.tellg();
		_file.seekg(currentPos);
		return (fileSize);
	}
	else
		return (0);
}

void	response::readWholeFile(std::vector<char> & store)
{
	size_t	fileSize = getResponseFileSize();
	size_t storePreviousSize = store.size();
	_file.seekg(0, _file.beg);
	store.resize(store.size() + fileSize);
	_file.read(&store[storePreviousSize], fileSize);
}


int	response::getStatus()
{
	return _status;
}

void response::setStatusLine(int status)
{
	_status = status;
	_statusLine = "HTTP/1.1";
	_statusLine += " ";
	_statusLine += to_string(status);
	_statusLine += " ";
	_statusLine += findStatus(status);
	_statusLine += "\r\n";
}

void response::setErrorMessage(int errorStatus, Rules &rules)
{
	setStatusLine(errorStatus);
	if (rules.errorPage.find(errorStatus) != rules.errorPage.end())
	{
		tryToOpenFile(rules.root + "/" + rules.errorPage[errorStatus]);
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Location", rules.errorPage[errorStatus]));
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Location", rules.errorPage[errorStatus]));
	}
	else
	{
		_body += defaultErrorMessage(errorStatus);
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Type", "text/plain"));
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Length", to_string(_body.length())));	
	}
}

void response::createAutoindexResponse( std::string& filePath )
{
	_body = autoIndex(filePath);
	setStatusLine(200);
	addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Type", "text/html"));
	addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Length", to_string(_body.length())));
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


std::string findContentType(std::string content)
{
	std::string extension(content, content.rfind('.') + 1, content.length());
	for (int i = 0; documentType[i].first != "END"; i++)
		if (extension == documentType[i].first)
			return (documentType[i].second);
	return (std::string());
}


std::string defaultErrorMessage(int errorStatus)
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


std::string to_string(int n)
{
	std::stringstream tmp;

	tmp << n;
	std::string result;

	tmp >> result;
	return (result);
}

/*

void response::tryToOpenAndReadFile(std::string filePath)
{
	std::string body;
	char buffer[1048];
	memset(&buffer[0], 0, 1048);

	//improve open and read
	std::ifstream file;
	file.open(filePath.c_str(), std::ios::in | std::ios::binary);
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
			file.close();
			return;
		}
		file.close();
	}
	else
		throw httpError(403);
	_body = body;
	if (file.good())
	{
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Type", findContentType(filePath)));
		addFieldToHeaderMap(std::make_pair<std::string, std::string> ("Content-Length", to_string(getResponseFileSize())));
	}
}

size_t response::continueReadingFile()
{
	size_t	fileSize = getResponseFileSize();
	size_t	currentPos = _file.tellg();
	size_t	numberOfBytesToRead = fileSize - currentPos;

	if (numberOfBytesToRead > 512)
		numberOfBytesToRead = 512;
	_file.read(_buffer, numberOfBytesToRead);
	if (_file.tellg() == static_cast<int>(fileSize))
		_hasBeenFullySent = true;
	return (numberOfBytesToRead);
}

size_t response::fillSendBuffer()
{
	size_t bufferSize = 0;
	if (_header.length())
	{
		bufferSize = _header.copy(_buffer, RESPONSE_BUFFER_SIZE, 0);
		_header.erase(0, bufferSize);
	}
	else
		bufferSize = continueReadingFile();
	return (bufferSize);

}
*/
