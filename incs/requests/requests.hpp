/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
/*   Updated: 2022/04/03 14:46:10 by pohl             ###   ########.fr       */
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
std::string eatWord(std::string & line);
bool fileExists(std::string file);
bool containsPort(std::string hostname);

class iRequest
{
	public:
//	int _status;
	static iRequest * createRequest(std::string &, const std::vector<ServerNode *> & ref);

	requestBase	_message;
//	response	_response;
	
	bool receivingisDone( void );
	virtual response createResponse( void ) = 0;
	virtual ~iRequest( void ) {};

	const std::string & getRequestURI( void ) const;
	
protected:
	const std::vector<ServerNode *> *_server;
	std::string _requestURI;

	std::string createFilePath( void );
	std::string createFileFromCgi( Rules& rules, std::string requestedFilePath, response& response );
	ServerNode * findServer( void );
private:
	std::string testIndexFile(std::string root, const std::vector<std::string> & indexList);
};

class getRequest : public iRequest
{
public:
	getRequest();
	~getRequest() {};

//	response createResponse();
//	void sendRequest(void) {};
	response createResponse();
	std::string	createResponseBody();
//private:
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest() {};


private:
//	void sendRequest(void) {};
	response createResponse();
//	bool requestURIisvalid();
};

class deleteRequest : public iRequest
{
	public:
	deleteRequest();
	~deleteRequest() {};

//	void	sendRequest(void) {};
	response createResponse();
};

#endif
