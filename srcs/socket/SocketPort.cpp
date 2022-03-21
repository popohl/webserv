/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPort.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 17:11:33 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketPort.hpp"

// +------------------------------------------+ //
//   CONSTRUCTOR OVERLOAD 					    //
// +------------------------------------------+ //

SocketPort::SocketPort(int const & port): ASocket()
{
	_port = port;
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == 0) //AF_INET pour utilisation en IPV4
	{
		std::perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY; // Default IP 0.0.0.0
	_address.sin_port = htons(port); //traduction du port
	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
}

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

SocketPort::SocketPort(void) : ASocket() {}

SocketPort::SocketPort (const SocketPort &other):
	ASocket(other){}

SocketPort::~SocketPort(void) {}

SocketPort &SocketPort::operator=(const SocketPort & rhs)
{
	ASocket::operator=(rhs);
	return *this;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //























// ASocket::ASocket(void) : _port(0), _socket_fd(0), _addrlen(sizeof(_address))
// {
// 	_address.sin_family = AF_INET;
// 	_address.sin_addr.s_addr = INADDR_ANY;
// 	_address.sin_port = htons(0);
// 	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
// }

// ASocket::ASocket (const ASocket &other):
// 	_port(other._port), _socket_fd(other._socket_fd),
// 	_address(other._address),
// 	_addrlen(other._addrlen)
// {}

// ASocket::~ASocket(void)
// {}

// ASocket &ASocket::operator=(const ASocket & rhs)
// {
// 	if (this != &rhs)
// 	{
// 		_address = rhs._address;
// 		_addrlen = rhs._addrlen;
// 	}
// 		return *this;
// }

// // +------------------------------------------+ //
// //   MEMBER FUNCTION					        //
// // +------------------------------------------+ //

// void	ASocket::bindSocket() const
// {
// 	  if (bind(_socket_fd, (struct sockaddr *)&_address, _addrlen)<0)
// 	{
// 		std::perror("Bind failed");
// 		exit(EXIT_FAILURE);
// 	}
// }

// void	ASocket::listenSocket() const
// {
// 	if (listen(_socket_fd, 10) < 0) // only 10 connection at the same time
// 	{
// 		std::perror("Listen failed");
// 		exit(EXIT_FAILURE);
// 	}
// }

// int	ASocket::getSocketFd() const
// { return (_socket_fd); }

// int	ASocket::getPort() const
// { return (_port); }
