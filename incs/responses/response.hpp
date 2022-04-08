// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
/*   response.hpp                                       :+:      :+:    :+:   */
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 09:50:59 by pcharton          #+#    #+#             //
/*   Updated: 2022/04/08 10:01:59 by pohl             ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <utility>
#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <fstream>
#include "configParsing/Parser.hpp"
#include "configParsing/Rules.hpp"

bool fileExists(std::string file);

//#include "requests/requests.hpp"

/*
**	Response =  Status-Line
**			    *(( general-header
**				 | response-header
**				 | entity-header ) CRLF)
**				CRLF
**				[ message-body ]
*/

struct response
{
public:
	response();
	response(const response & src);
	response & operator = (const response & src);
	~response();
	
	std::vector<char>	createFormattedResponse();
	void		addFieldToHeaderMap(std::pair<std::string, std::string>input);
	void		replaceFieldToHeaderMap(std::pair<std::string, std::string>input);

	void		tryToOpenFile(std::string filePath);
	void		tryToOpenAndReadFile(std::string RequestUri);

	void		setErrorMessage(int errorStatus, Rules & rules);
	void		readWholeFile(std::vector<char> & store);

	void		createAutoindexResponse( std::string& filePath );
	int			getStatus();
	void		setStatusLine(int status);
	void		printStatus();

	void		printHeader();

private:
	void		createHeader();
	size_t		getResponseFileSize();
	std::map<std::string, std::string> _headerFields;
	int			_status;
	std::string _statusLine;
	std::string	_header;
	std::string	_body;
	std::ifstream	_file;
	Rules		_rules;


};

// Utils

std::string to_string(int n);
std::string defaultErrorMessage(int errorStatus);
std::string autoIndex(std::string	root);
std::string findContentType(std::string content);
std::string findStatus(int status);

//Custom exceptions

class fileNotFound : public std::exception
{
public:
	fileNotFound();
	virtual const char * what() const throw();
};

class fileCouldNotBeOpen : public std::exception
{
public:
	fileCouldNotBeOpen();
	virtual const char * what() const throw();
};


#endif
