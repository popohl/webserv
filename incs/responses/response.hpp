// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.hpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 09:50:59 by pcharton          #+#    #+#             //
//   Updated: 2022/03/29 14:17:25 by pcharton         ###   ########.fr       //
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
//std::string to_string(std::streamsize n);

class fileNotFound : public std::exception
{
public:
	fileNotFound();
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

public:

	response();
	response(const response & src);
	response & operator = (const response & src);
	~response();

	std::string	createFormattedResponse();
	void		addFieldToHeaderMap(std::pair<std::string, std::string>input);
	void		tryToOpenAndReadFile(std::string RequestUri);
	void		setStatusLine();	

private:	
	void		createHeader();

};

#endif
