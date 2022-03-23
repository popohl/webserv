/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketPort.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 11:57:56 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKECTPORT_HPP
#define SOCKECTPORT_HPP

#include "ASocket.hpp"

class SocketPort : public ASocket
{
	protected:
		SocketPort();

	public:

		SocketPort(int port);
		SocketPort(SocketPort const & other );
		virtual ~SocketPort();

		SocketPort & operator=(SocketPort const & other);

		void	bindSocket() const;
		void	listenSocket() const;

		struct sockaddr_in	_address; // it's public otherwise we have to make a geter function on is address
		int					_addrlen;
};

#endif
