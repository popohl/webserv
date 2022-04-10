// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
/*   requestBase.hpp                                    :+:      :+:    :+:   */
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:49:25 by pcharton          #+#    #+#             //
//   Updated: 2022/04/08 19:37:56 by pcharton         ###   ########.fr       //
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


struct requestBase {

	requestBase();
	void	parseRequest(std::vector<char> &data);
	void	parseHeader(std::string & input);
	void	parseBody(std::vector<char> &data);
	bool containsHostField(void);
	
	bool _headerFinished;
	bool _bodyFinished;
	int _status;


	std::vector<char>	_unfinishedData;
	std::map<std::string, std::string> _header;
	size_t		_bodySize;
	size_t		_bodyExpectedSize;
	std::vector<char> _body;

private:
	std::pair<std::string, std::string>splitIntoPair(std::string line);

	void updateResponseStatus(void);
	size_t	findBodyLength(void);
	void	eatCRLF(std::vector<char> & data);
	
	//chunked transfer utils

	std::deque<size_t> _chunksList;

	size_t	eatChunkSize(std::vector<char> & data);
	void	processChunk(std::vector<char> & data);
	size_t	dataContainsCRLF(const std::vector<char> & data);
	size_t	findCRLFPositionInData(const std::vector<char> & data);
	
	//parsing header functions
	std::string	removeOneHeaderLineFromInput(std::string & input);
	bool	HeaderLineIsCorrectlyFormatted(const std::string & line);
	bool	lineIsHeaderEnd(const std::string & line);
};

bool isHeaderEnd(const char *input);

struct chunk
{
	bool				_sizeDelimiterFound;
	bool				_chunkDelimiterFound;
	bool				_chunkIsDone;
	size_t				_chunkSize;
	std::vector<char>	_chunkData;

	void				eatChunkSize(std::vector<char> & data);
	void				eatCRLF(std::vector<char> & data);
	void				tryToEatChunkData(std::vector<char> & data);
};

std::vector<char> transformChunkListIntoData(std::deque<chunk> & list);
#endif
