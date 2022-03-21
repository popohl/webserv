/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 17:43:24 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketClient.hpp"

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //

SocketClient::SocketClient(int const & port, int const & fd): ASocket()
{
	_port = port;
	_socket_fd = fd;
	_response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 202\n\n"
		"<form method=\"post\" enctype=\"multipart/form-data\"><div><label for=\"file\">select file</label><input type=\"file\" id=\"file\" name=\"file\" multiple></div><div><button>Envoyer</button></div></form>";
}

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

SocketClient::SocketClient(void) : ASocket() {}

SocketClient::SocketClient (const SocketClient &other):
	ASocket(other), _response(other._response) {} // copier le buffer caractere par caractere

SocketClient::~SocketClient(void) {}

SocketClient &SocketClient::operator=(const SocketClient & rhs) // ici aussi
{
	ASocket::operator=(rhs);
	_response = rhs._response;
	return *this;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //

void	SocketClient::addContent(char *content)
{
	std::string	temp;

	temp = content;
	_header += temp;
	std::cout << _header << std::endl;
}

std::string	SocketClient::getResponse()
{ return (_response); }
