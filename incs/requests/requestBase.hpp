// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestBase.hpp                                    :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:49:25 by pcharton          #+#    #+#             //
//   Updated: 2022/04/05 11:44:54 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef REQUESTBASE_HPP
# define REQUESTBASE_HPP

#include <string>
#include <vector>
#include <map>
#include <list>
#include <deque>

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

struct requestBase {

	requestBase();
	void	parseRequest(const std::string &line);
	void	parseHeader(std::string & input);
	void	parseBody(std::string &line);

	bool containsHostField(void);
	
	bool _headerFinished;
	bool _bodyFinished;
	int _status;

	std::string	_unfinishedField;
	std::map<std::string, std::string> _header;
	size_t		_bodySize;
	size_t		_bodyExpectedSize;
	std::string _body;

private:
	std::pair<std::string, std::string>splitIntoPair(std::string line);

	void updateResponseStatus(void);
	size_t	findBodyLength(void);

	//chunked transfer utils
	std::deque<size_t> _chunksList;
	size_t	eatChunkSize(std::string & line);
	void	processChunk(std::string & line);
	
	//parsing header functions
	std::string	removeOneHeaderLineFromInput(std::string & input);
	bool	HeaderLineIsCorrectlyFormatted(const std::string & line);
	bool	lineIsHeaderEnd(const std::string & line);
};

bool isHeaderEnd(const char *input);


//requestHeaderToken::field findField(const char *);

#endif
