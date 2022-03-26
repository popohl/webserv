/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
//   Updated: 2022/03/26 17:09:31 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

//#include "requests/requestHeaderToken.hpp"
#include "requests/parsing.hpp"
#include "requests/requestHeaderToken.hpp"
#include "responses/response.hpp"

const std::pair<std::string, std::string>responseStatus[] = {
	std::make_pair("100", "Continue"),
	std::make_pair("200", "OK"),
	std::make_pair("400", "Bad Request"),
	
	std::make_pair("405", "Method Not Allowed"),
};


std::string date();

class iRequest
{
	public:
//	virtual void parse(void) = 0;
//	virtual void sendRequest(void) = 0;
	static iRequest * createRequest(std::string &);

	requestBase	_message;
//	int _status;
	bool receivingisDone();
	virtual std::string createResponse() = 0;
//	std::vector<requestHeaderToken> parseRequestHeader(const char *input);

	virtual ~iRequest() {};

};

class getRequest : public iRequest
{
	//get request has no body !
public:
	getRequest();
	~getRequest() {};

//	response createResponse();
	void parse(void) {};
	void sendRequest(void) {};
	std::string createResponse();
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest() {};

	void parse(void) {};
	void sendRequest(void) {};
	std::string createResponse();
};

class deleteRequest : public iRequest
{
	public:
	deleteRequest();
	~deleteRequest() {};
	void parse(void) {};
	void sendRequest(void) {};
	std::string createResponse();
};

#endif
