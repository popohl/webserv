/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/07 14:23:29 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/SocketClient.hpp"

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //

SocketClient::SocketClient(int port, int fd, const std::vector<ServerNode *> & ref):
	ASocket(port, fd, CLIENT, ref), _response(), _request (NULL), _responseStatus(), _totalSend()
{ time(&_timer); }

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

SocketClient::SocketClient(void) : ASocket(), _response(), _request(NULL), _responseStatus(),_totalSend(0) {}

SocketClient::~SocketClient(void)
{
	if (_request)
		delete _request;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //

std::vector<char>	SocketClient::getResponse() const
{ return (_response); }

void SocketClient::setResponse(std::vector<char> input)
{ _response = input; }

void	SocketClient::clearAll()
{
	_response.clear();
	delete _request;
	_request = NULL;
}

bool	SocketClient::checkTimeout() const
{
	time_t now;
	double ret;

	time(&now);
	ret = difftime(now, _timer);
	if (ret > CLIENT_TIMEOUT)
		return(true);
	else
		return (false);
}

void	SocketClient::resetTimer()
{ time(&_timer); }

void	SocketClient::erasePartResponse(int i)
{ _response.erase(_response.begin(), _response.begin() + i); }
