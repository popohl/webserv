/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 15:09:05 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLIENT_CLIENT
#define SOCKETCLIENT_CLIENT


#define PORT 1
#define CLIENT 2
#include "ASocket.hpp"

class SocketClient : public ASocket
{
	protected:
		SocketClient();
		std::string	_header;
		std::string	_response;

	public:
		SocketClient(int port, int fd);
		SocketClient(SocketClient const & other );
		virtual ~SocketClient();

		SocketClient & operator=(SocketClient const & other);

		void	addContent(char *content);
		void	clearAll();
		std::string	getResponse();
};

#endif
