// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.hpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 09:50:59 by pcharton          #+#    #+#             //
//   Updated: 2022/03/29 10:35:49 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <utility>
#include <string>
#include <map>
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
//	void buildResponseMessage(iRequest *);

private:
	std::map<std::string, std::string> _headerFields;
//	int			_status;
	std::string _statusLine;
	std::string	_header;
	std::string	_body;

public:
	std::string createFormattedResponse()
		{
			std::string result;

			result = _statusLine;
			result += "\r\n";
			if (!_header.length())
				createHeader();
			result += _header;
			result += "\r\n";
			result += _body;

			return (result);
		}

	void addFieldToHeaderMap(std::pair<std::string, std::string>input)
		{
			if (_headerFields.find(input.first) == _headerFields.end())
				_headerFields.insert(input);
			else
				_headerFields[input.first] += input.second;
		}

private:
	void	createHeader()
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
};

#endif
