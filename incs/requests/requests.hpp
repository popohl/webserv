/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
//   Updated: 2022/04/06 14:12:03 by pcharton         ###   ########.fr       //
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
	static		iRequest * createRequest(std::string &, const std::vector<ServerNode *> & ref);
	virtual		response createResponse() = 0;
	
	requestBase	_message;
	
	bool		receivingisDone();

	void		printRequest();
	const		std::string & getRequestURI();
	
protected:
	const		std::vector<ServerNode *> *_server;
	std::string _requestURI;

	std::string createFilePath();
	ServerNode * findServer();
	std::string	testIndexFile(std::string root, const std::vector<std::string> & indexList);
	
private:
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
private:
	bool	isAutoindex(const Rules &);
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest();

	response createResponse();
private:
	std::string createPostedFilePath(const std::string & root, const std::string & requestURI);
};

class deleteRequest : public iRequest
{
	public:
	deleteRequest();
	~deleteRequest();

	response createResponse();
};

#endif
