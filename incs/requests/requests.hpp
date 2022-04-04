/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             */
/*   Updated: 2022/04/04 17:45:34 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

//#include "requests/requestHeaderToken.hpp"
#include "requests/requestBase.hpp"
#include "responses/response.hpp"
#include "configParsing/Parser.hpp"

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

	bool receivingisDone();
	virtual response createResponse() = 0;
	virtual std::string printType() = 0;
	virtual ~iRequest() {};

	const std::string & getRequestURI();
	ServerNode * findServer(); // DEPLACE EN PUBLIC

protected:
	const std::vector<ServerNode *> *_server;
	std::string _requestURI;

	std::string createFilePath();
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
	std::string printType() {return ("GET");};

//private:
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest() {};

	std::string printType() {return ("POST");};

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

	std::string printType() {return ("DELETE");};

//	void	sendRequest(void) {};
	response createResponse();
};

#endif
