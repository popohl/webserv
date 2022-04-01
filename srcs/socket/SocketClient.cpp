/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/01 15:56:22 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/SocketClient.hpp"

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //

SocketClient::SocketClient(int port, int fd, const std::vector<ServerNode *> & ref): ASocket(port, fd, CLIENT, ref), _response(), _request (NULL)
{
	time(&_timer);
	// _response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 202\n\n"
	// 	"<form method=\"post\" enctype=\"multipart/form-data\"><div><label for=\"file\">select file</label><input type=\"file\" id=\"file\" name=\"file\" multiple></div><div><button>Envoyer</button></div></form>";
	//	_response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
}

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

SocketClient::SocketClient(void) : ASocket(), _response(), _request(NULL) {}

SocketClient::~SocketClient(void)
{
	if (_request)
		delete _request;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //

std::string	SocketClient::getResponse() const
{ return (_response); }

void SocketClient::setResponse(std::string input)
{ _response += input; }

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
	ret = difftime(_timer, now);
	std::cout << "client " << _socket_fd << " is running for " << ret << " secondes" <<std::endl;
	if (ret > CLIENT_TIMEOUT)
		return(true);
	else
		return (false);
}

void	SocketClient::resetTimer()
{ time(&_timer); }

void	SocketClient::erasePartResponse(int i)
{ _response.erase(0,i); }
