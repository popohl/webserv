// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestBase.hpp                                    :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:49:25 by pcharton          #+#    #+#             //
//   Updated: 2022/03/31 14:24:23 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef REQUESTBASE_HPP
# define REQUESTBASE_HPP

#include <string>
#include <vector>
#include <map>
#include <list>

#include "responses/response.hpp"

class malformedHeader : public std::exception
{
public:
	malformedHeader();
	virtual const char * what () const throw();
};

class unfinishedHeader : public std::exception
{
public:
	unfinishedHeader();
	virtual const char * what () const throw();
};

class fieldAlreadyExists : public std::exception
{
public:
	fieldAlreadyExists();
	virtual const char * what () const throw();
};


struct requestHeaderToken {

	requestHeaderToken();
	requestHeaderToken(const std::pair <std::string, std::string> & rhs);

	std::pair<std::string, std::string> _token;
};

requestHeaderToken parseHost(std::string);
std::vector<requestHeaderToken> parseRequestHeader(const char *input);

std::list<std::string>split_header_to_lines(const std::string &input, size_t &headerSize);

requestHeaderToken treatLine(std::string line);

struct requestBase {

	requestBase();
	void parseRequest(const std::string &line);
	size_t parseHeader(std::string &line);
	void parseBody(const std::string &line);

	bool _headerFinished;
	bool _bodyFinished;
	int _status;
	
	std::map<std::string, std::string> _header;
	size_t		_bodySize;
	size_t		_bodyExpectedSize;
	std::string _body;

private:
	std::pair<std::string, std::string>splitIntoPair(std::string line);
	bool containsHostField(void);
	void updateResponseStatus(void);
	size_t	findBodyLength(void);
};

bool isHeaderEnd(const char *input);


//requestHeaderToken::field findField(const char *);

#endif
