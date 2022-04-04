/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
//   Updated: 2022/04/04 16:48:22 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLIENT_CLIENT
#define SOCKETCLIENT_CLIENT


#define PORT 1
#define CLIENT 2
#define CLIENT_TIMEOUT 90
#include "ASocket.hpp"
#include "requests/requests.hpp"
#include "configParsing/Parser.hpp"

class SocketClient : public ASocket
{
	protected:
		SocketClient();

		std::vector<unsigned char> _response;
		time_t		_timer;

	public:
		SocketClient(int port, int fd, const std::vector<ServerNode *> & ref);
		virtual ~SocketClient();

		void		clearAll();
	std::vector<unsigned char>	getResponse() const;
		void		setResponse(std::vector<unsigned char> input);
		void		resetTimer();
		bool		checkTimeout()const;
		void		erasePartResponse(int i);

		iRequest *		_request; // allocation et suppression a chaque boucle de lecture/ecriture
};

#endif
