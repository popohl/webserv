// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requests.hpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 10:43:44 by pcharton          #+#    #+#             //
//   Updated: 2022/03/15 12:39:20 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef REQUESTS_HPP
# define REQUESTS_HPP

#include "parsing.hpp"

class iRequest
{
	virtual void parse(void) = 0;
	virtual void sendRequest(void) = 0;
	iRequest * createRequest(std::string);
};


class getRequest : public iRequest
{
	getRequest();
	~getRequest();

	void parse(void);
	void sendRequest(void);
};

class postRequest : public iRequest
{
	postRequest();
	~postRequest();

	void parse(void);
	void sendRequest(void);

};

class deleteRequest : public iRequest
{
	deleteRequest();
	~deleteRequest();
	void parse(void);
	void sendRequest(void);

};
#endif
