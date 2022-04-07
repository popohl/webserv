/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
//   Updated: 2022/04/07 10:17:04 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

//#include "requests/requestHeaderToken.hpp"
#include "requests/requestBase.hpp"
#include "responses/response.hpp"
#include "configParsing/Parser.hpp"

std::string date();
bool fileExists(std::string file);

class iRequest
{
	public:

	virtual		~iRequest();
	static		iRequest * createRequest(std::vector<unsigned char> &, const std::vector<ServerNode *> & ref);
	virtual		response createResponse() = 0;
	virtual		std::string printType() = 0;

	requestBase	_message;
//	response	_response;

	bool receivingisDone();
	const std::string & getRequestURI();
	ServerNode * findServer();
	void		printRequest();

protected:
	const		std::vector<ServerNode *> *_server;
	std::string _requestURI;
	std::string _httpVersion;
	std::string createFilePath();
	std::string	testIndexFile(std::string root, const std::vector<std::string> & indexList);

	static bool	methodIsValid(const std::string & method);
	static bool	requestURIIsValid(const std::string & requestURI);
	static bool	httpVersionIsValid(const std::string & httpVersion);

private:
	static iRequest * allocateRequest(const std::string & method, const std::string & requestURI, const std::string & httpVersion);
	static std::string eatWord(std::string & line);
	bool		containsPort(std::string hostname);
};

class getRequest : public iRequest
{
public:
	getRequest();
	~getRequest();

	response	createResponse();
	std::string	createResponseBody();
	std::string printType();

private:
	bool	isAutoindex(const Rules &);
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest();

	response createResponse();
	std::string printType();

private:
	std::string createPostedFilePath(const std::string & root, const std::string & requestURI);
};

class deleteRequest : public iRequest
{
public:
	deleteRequest();
	~deleteRequest();

	response createResponse();
	std::string printType();
};

class errorRequest : public iRequest
{
public:
	errorRequest();
	~errorRequest();

	response createResponse();
	std::string printType();
};

#endif
