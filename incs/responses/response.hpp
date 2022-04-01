// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.hpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 09:50:59 by pcharton          #+#    #+#             //
//   Updated: 2022/04/01 17:50:55 by pcharton         ###   ########.fr       //
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

//#include "requests/requests.hpp"

/*
**	Response =  Status-Line
**			    *(( general-header
**				 | response-header
**				 | entity-header ) CRLF)
**				CRLF
**				[ message-body ]
*/

std::string to_string(int n);
std::string defaultErrorMessage(int errorStatus);
//std::string to_string(std::streamsize n);

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

struct response
{
//	void buildResponseMessage(iRequest *);
private:
	std::map<std::string, std::string> _headerFields;
	int			_status;
	std::string _statusLine;
	std::string	_header;
	std::string	_body;

	Rules		_rules;
//	ServerNode * _server;

public:
	response();
//	response(Rules rules);
	response(const response & src);
	response & operator = (const response & src);
	~response();

	std::string	createFormattedResponse();
	void		addFieldToHeaderMap(std::pair<std::string, std::string>input);
	void		tryToOpenAndReadFile(std::string RequestUri);
	void		setStatusLine(int status);	
	void		setErrorMessage(int errorStatus, Rules & rules);

private:
	void		createHeader();

};

#endif
