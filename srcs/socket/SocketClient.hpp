/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 18:02:12 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLIENT_CLIENT
#define SOCKETCLIENT_CLIENT

#include "ASocket.hpp"

class SocketClient : public ASocket
{
	protected:
		SocketClient();
		std::string	_header;
		std::string	_response;

	public:
		SocketClient(int const & port, int const & fd);
		SocketClient(SocketClient const & other );
		virtual ~SocketClient();

		SocketClient & operator=(SocketClient const & other);

		void	addContent(char *content);
		std::string	getResponse();
};

#endif
