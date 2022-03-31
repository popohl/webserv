/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/31 12:16:37 by fmonbeig         ###   ########.fr       */
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
		std::string	_header; //NOTA BENE le header est il toujours nécessaire ?
		std::string	_response;
		time_t		_timer;

	public:
		SocketClient(int port, int fd, ServerNode * ref);
		SocketClient(SocketClient const & other );
		virtual ~SocketClient();

		SocketClient & operator=(SocketClient const & other);

		void		addContent(char *content); //FIXME a virer
		void		clearAll();
		std::string	getResponse() const;
		void		setResponse(std::string input);
		void		resetTimer();
		bool		checkTimeout()const;

		//prepareResponse(iRequest *);
		ServerNode *	_server;
		iRequest *		_request; // allocation et suppression a chaque boucle de lecture/ecriture

};

#endif
