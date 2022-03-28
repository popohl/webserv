/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/24 17:21:22 by fmonbeig         ###   ########.fr       */
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
		const int	_port;
		const int	_socket_fd;
		const int	_type;

	public:

		ASocket();
		ASocket(int port, int fd, int type);
		ASocket(ASocket const & other);
		virtual ~ASocket();

		ASocket & operator=(ASocket const & other);

		int	getSocketFd() const;
		int	getPort() const;
		int	getType() const;

		class FailCreation: public std::exception{
			public:
				virtual const char* what() const throw();
		};
};

#endif
