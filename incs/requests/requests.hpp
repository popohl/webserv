// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.hpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             //
//   Updated: 2022/03/15 20:24:07 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

#include "requests/parsing.hpp"

class iRequest
{
	public:
	virtual void parse(void) = 0;
//	virtual void sendRequest(void) = 0;
	static iRequest * createRequest(std::string);
	virtual ~iRequest() {};
};

class getRequest : public iRequest
{
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
