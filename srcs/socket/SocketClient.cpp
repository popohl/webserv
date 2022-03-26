/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
//   Updated: 2022/03/26 16:10:49 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "socket/SocketClient.hpp"

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //

SocketClient::SocketClient(int port, int fd): ASocket(port, fd, CLIENT), _response(), _request (NULL)
{
	// _response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 202\n\n"
	// 	"<form method=\"post\" enctype=\"multipart/form-data\"><div><label for=\"file\">select file</label><input type=\"file\" id=\"file\" name=\"file\" multiple></div><div><button>Envoyer</button></div></form>";
//	_response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
}

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

SocketClient::SocketClient(void) : ASocket(), _response(), _request(NULL) {}

SocketClient::SocketClient (const SocketClient &other):
	ASocket(other), _response(other._response), _request(other._request) {} // copier le buffer caractere par caractere et rajouter request

SocketClient::~SocketClient(void) {}

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

void	SocketClient::addContent(char *content)
{
	std::string	temp;

	temp = content;
	_header.append(temp, 0, temp.size());
	std::cout << _header << std::endl;
}

std::string	SocketClient::getResponse()
{ return (_response); }

void SocketClient::setResponse(std::string input)
{ _response += input; }

void	SocketClient::clearAll()
{
	_header.clear();
	_response.clear();
	/****
	 * delete _request;
	 *	_request = NULL;
	 * */
}
