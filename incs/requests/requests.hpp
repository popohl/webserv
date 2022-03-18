// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.hpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             //
//   Updated: 2022/03/18 11:54:10 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

//#include "requests/requestHeaderToken.hpp"
#include "requests/parsing.hpp"

class iRequest
{
	public:
	virtual void parse(void) = 0;
//	virtual void sendRequest(void) = 0;
	static iRequest * createRequest(std::string);

//	std::vector<requestHeaderToken> parseRequestHeader(const char *input);

	virtual ~iRequest() {};

};

class getRequest : public iRequest
{
	//get request has no body !
public:
	getRequest();
	~getRequest() {};

	void parse(void) {};
	void sendRequest(void) {};
};

class postRequest : public iRequest
{
	public:
	postRequest();
	~postRequest() {};

	void parse(void) {};
	void sendRequest(void) {};

};

class deleteRequest : public iRequest
{
	public:
	deleteRequest();
	~deleteRequest() {};
	void parse(void) {};
	void sendRequest(void) {};

};

#endif
