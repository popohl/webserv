/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
//   Updated: 2022/03/30 12:22:04 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "socket/ASocket.hpp"

std::vector<ServerNode *>badHack;

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //


ASocket::ASocket(int port, int fd, int type, const std::vector<ServerNode *> &ref):
	_port(port), _socket_fd(fd), _type(type), _servers(ref)
{}

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //


ASocket::ASocket(void) : _port(0), _socket_fd(0), _type(0), _servers(badHack)
{}

ASocket::ASocket (const ASocket &other):
	_port(other._port), _socket_fd(other._socket_fd),
	_type(other._type), _servers(other._servers)
{}

ASocket::~ASocket(void)
{}

ASocket &ASocket::operator=(const ASocket & rhs)
{
	(void)rhs;
	return *this;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //

int	ASocket::getSocketFd() const
{ return (_socket_fd); }

int	ASocket::getPort() const
{ return (_port); }

int	ASocket::getType() const
{ return (_type); }

const char* ASocket::FailCreation::what() const throw()
{
	return ("\e[0;31mERROR: Fail on created the socket\e[0m");
}
