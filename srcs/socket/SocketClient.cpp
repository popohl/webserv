/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/31 14:35:15 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/SocketClient.hpp"

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //
//FIXME bien gerer les constructeur par copy des variables
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
	ASocket(other), _response(other._response), _server(other._server),_request(other._request) {} // copier le buffer caractere par caractere et rajouter request

SocketClient::~SocketClient(void)
{
	if (_request)
		delete _request;
}

SocketClient &SocketClient::operator=(const SocketClient & rhs) // ici aussi
{
	ASocket::operator=(rhs);
	_response = rhs._response;
	_request = rhs._request;
	return *this;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //

void	SocketClient::addContent(char *content) //Fonction peut etre pas nécessaire
{
	std::string	temp;

	temp = content;
	_header.append(temp, 0, temp.size());
	std::cout << _header << std::endl;
}

std::string	SocketClient::getResponse() const
{ return (_response); }

void SocketClient::setResponse(std::string input)
{ _response += input; }

void	SocketClient::clearAll()
{
	_header.clear();
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
