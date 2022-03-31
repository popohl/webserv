/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPort.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
//   Updated: 2022/03/30 12:16:46 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETPORT_HPP
#define SOCKETPORT_HPP

#include "configParsing/Parser.hpp"
#include "ASocket.hpp"

#define PORT 1
#define CLIENT 2

class SocketPort : public ASocket
{
	protected:

	public:
		SocketPort();

	SocketPort(int port, const std::vector<ServerNode *> & _server);
		SocketPort(SocketPort const & other );
		virtual ~SocketPort();

		SocketPort & operator=(SocketPort const & other);

		bool	bindSocket() const;
		bool	listenSocket() const;

		struct sockaddr_in	_address; // it's public otherwise we have to make a geter function on is address
		int					_addrlen;
	
//		ServerNode * _server;
//		std::vector<ServerNode *> _servers;
};

#endif
