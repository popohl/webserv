/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
/*   Updated: 2022/04/04 19:48:02 by pohl             ###   ########.fr       */
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

	virtual ~iRequest();
	static iRequest * createRequest(std::string &, const std::vector<ServerNode *> & ref);
	virtual response createResponse() = 0;
	
	requestBase	_message;
	
	bool receivingisDone();


	const std::string & getRequestURI( void ) const;
	
protected:
	const std::vector<ServerNode *> *_server;
	std::string _requestURI;

	std::string createFilePath( void );
	std::string createFileFromCgi( Rules& rules, std::string requestedFilePath, response& response );
	ServerNode * findServer( void );
private:
	std::string testIndexFile(std::string root, const std::vector<std::string> & indexList);
	static std::string eatWord(std::string & line);
	bool containsPort(std::string hostname);
};

class getRequest : public iRequest
{
public:
	getRequest();
	~getRequest();

	response createResponse();
	std::string	createResponseBody();
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
