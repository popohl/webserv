/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 15:56:34 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASocket_HPP
#define ASocket_HPP

#include <iostream>
#include <cstring>
#include <cerrno>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <fcntl.h>

class ASocket
{
	protected:
		int	_port;
		int	_socket_fd;

	public:

		ASocket();
		ASocket(ASocket const & other );
		virtual ~ASocket();

		ASocket & operator=(ASocket const & other);

		int	getSocketFd() const;
		int	getPort() const;

		void	bindSocket() const;
		void	listenSocket() const;

		struct sockaddr_in	_address; // it's public otherwise we have to make a geter function on is address
		int					_addrlen;
};

#endif
