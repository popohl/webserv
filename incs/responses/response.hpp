// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.hpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/25 09:50:59 by pcharton          #+#    #+#             //
//   Updated: 2022/03/25 14:59:08 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <utility>
#include <string>
#include "requests/requests.hpp"

/*
**	Response =  Status-Line
**			    *(( general-header
**				 | response-header
**				 | entity-header ) CRLF)
**				CRLF
**				[ message-body ]
*/

//maybe change array for another container
const std::pair<std::string, std::string>responseStatus[] = {
	std::make_pair("100", "Continue"),
	std::make_pair("200", "OK"),
	std::make_pair("400", "Bad Request"),
	std::make_pair("504", "Method Not Allowed"),
};

struct response
{
//	void buildResponseMessage(iRequest *);
	std::string _message;
};

#endif
