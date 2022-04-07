/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
/*   Updated: 2022/04/07 10:32:44 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

//#include "requests/requestHeaderToken.hpp"
#include "requests/requestBase.hpp"
#include "responses/response.hpp"
#include "configParsing/Parser.hpp"
#include "configParsing/Rules.hpp"

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
	std::string createFilePath( Rules& rules );
	std::string	testIndexFile(std::string root, const std::vector<std::string> & indexList);
	std::string createFileFromCgi( Rules& rules, std::string requestedFilePath, response& response );

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
	std::string printType() {return ("GET");};

//private:
private:
	bool	isAutoindex(const Rules &);
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest();

	std::string printType() {return ("POST");};

private:
//	void sendRequest(void) {};
	response createResponse();
private:
	std::string createPostedFilePath(const std::string & root, const std::string & requestURI);
};

class deleteRequest : public iRequest
{
	public:
	deleteRequest();
	~deleteRequest();

	std::string printType() {return ("DELETE");};

//	void	sendRequest(void) {};
	response createResponse();
};

#endif
