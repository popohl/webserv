/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
//   Updated: 2022/03/26 14:29:09 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLIENT_CLIENT
#define SOCKETCLIENT_CLIENT


#define PORT 1
#define CLIENT 2
#include "ASocket.hpp"
#include "requests/requests.hpp"

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
	void	setResponse(std::string input);
		//prepareResponse(iRequest *);
		iRequest * _request; // allocation et suppression a chaque boucle de lecture/ecriture
};

#endif
