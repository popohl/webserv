/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
//   Updated: 2022/03/29 10:37:57 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

//#include "requests/requestHeaderToken.hpp"
#include "requests/requestBase.hpp"
#include "responses/response.hpp"
#include "configParsing/Parser.hpp"

const std::pair<std::string, std::string>responseStatus[] = {
	std::make_pair("100", "Continue"),
	std::make_pair("200", "OK"),
	std::make_pair("400", "Bad Request"),
	
	std::make_pair("405", "Method Not Allowed"),
};


std::string date();
std::string eatWord(std::string & line);

class iRequest
{
	public:
//	int _status;
	static iRequest * createRequest(std::string &, ServerNode *);

	requestBase	_message;
	response	_response;
	
	bool receivingisDone();
	virtual std::string createResponse() = 0;
	virtual ~iRequest() {};

	const std::string & getRequestURI();
	
protected:
	ServerNode	*_server;
	std::string _requestURI;
};

class getRequest : public iRequest
{
	//get request has no body !
public:
	getRequest();
	~getRequest() {};

//	response createResponse();

	void sendRequest(void) {};
	std::string createResponse();
	std::string	createResponseBody();
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest() {};


	void sendRequest(void) {};
	std::string createResponse();
};

class deleteRequest : public iRequest
{
	public:
	deleteRequest();
	~deleteRequest() {};

	void	sendRequest(void) {};
	std::string createResponse();
};

#endif
