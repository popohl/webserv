/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/01 11:27:42 by fmonbeig         ###   ########.fr       */
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
		SocketClient(SocketClient const & other ); //NOTA BENE Pas besoin des constructeurs par copy et de la surcharge d'operateur =
		SocketClient & operator=(SocketClient const & other);
		std::string	_response;
		time_t		_timer;

	public:
		SocketClient(int port, int fd, ServerNode * ref);
		virtual ~SocketClient();

		void		clearAll();
		std::string	getResponse() const;
		void		setResponse(std::string input);
		void		resetTimer();
		bool		checkTimeout()const;
		void		erasePartResponse(int i);

		//prepareResponse(iRequest *);
		ServerNode *	_server;
		iRequest *		_request; // allocation et suppression a chaque boucle de lecture/ecriture

};

#endif
