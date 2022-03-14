/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/14 14:25:50 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <cstring>
#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>

class Socket
{
	protected:

		const int			_port;
		int					_socket_fd;
		Socket();

	public:

		Socket(int const & port);
		Socket(Socket const & other );
		virtual ~Socket();

		Socket & operator=(Socket const & other);

		void	bindSocket() const;
		void	listenSocket() const;

		int	getSocketFd() const;
		int	getPort() const;

		struct sockaddr_in	_address; // it's public otherwise we have do make a geter fucntion on is address
		int					_addrlen;
};

#endif
