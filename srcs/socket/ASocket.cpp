/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 15:46:34 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASocket.hpp"


// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

ASocket::ASocket(void) : _port(0), _socket_fd(0), _addrlen(sizeof(_address))
{}

ASocket::ASocket (const ASocket &other):
	_port(other._port), _socket_fd(other._socket_fd),
	_address(other._address),
	_addrlen(other._addrlen)
{}

ASocket::~ASocket(void)
{}

ASocket &ASocket::operator=(const ASocket & rhs)
{
	if (this != &rhs)
	{
		_address = rhs._address;
		_addrlen = rhs._addrlen;
	}
		return *this;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //

void	ASocket::bindSocket() const
{
	  if (bind(_socket_fd, (struct sockaddr *)&_address, _addrlen)<0)
	{
		std::perror("Bind failed");
		exit(EXIT_FAILURE);
	}
}

void	ASocket::listenSocket() const
{
	if (listen(_socket_fd, 10) < 0) // only 10 connection at the same time
	{
		std::perror("Listen failed");
		exit(EXIT_FAILURE);
	}
}

int	ASocket::getSocketFd() const
{ return (_socket_fd); }

int	ASocket::getPort() const
{ return (_port); }
