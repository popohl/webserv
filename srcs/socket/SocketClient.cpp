/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/31 18:00:43 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/SocketClient.hpp"

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //

SocketClient::SocketClient(int port, int fd, ServerNode * ref):
	ASocket(port, fd, CLIENT), _response(),
	_server(ref), _request (NULL)
{
	time(&_timer);
	// _response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 202\n\n"
	// 	"<form method=\"post\" enctype=\"multipart/form-data\"><div><label for=\"file\">select file</label><input type=\"file\" id=\"file\" name=\"file\" multiple></div><div><button>Envoyer</button></div></form>";
	//	_response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
}

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

SocketClient::SocketClient(void) : ASocket(), _response(), _server(NULL), _request(NULL) {}

SocketClient::SocketClient (const SocketClient &other):
	ASocket(other), _response(other._response), _server(other._server),_request(other._request) {}

SocketClient::~SocketClient(void)
{
	if (_request)
		delete _request;
}

SocketClient &SocketClient::operator=(const SocketClient & rhs) // ici aussi
{
	ASocket::operator=(rhs);
	_response = rhs._response;
	return *this;
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
