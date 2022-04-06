/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/04 13:58:33 by fmonbeig         ###   ########.fr       */
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
#include <time.h>

#include "configParsing/Parser.hpp"

class ASocket
{
	protected:
		const int	_port;
		const int	_socket_fd;
		const int	_type;
	public:
		const std::vector<ServerNode *> & _servers;

		ASocket();
		ASocket(int port, int fd, int type, const std::vector<ServerNode *> & ref);
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
