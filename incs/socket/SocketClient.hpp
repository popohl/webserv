/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
//   Updated: 2022/03/30 11:07:15 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETCLIENT_CLIENT
#define SOCKETCLIENT_CLIENT


#define PORT 1
#define CLIENT 2
#include "ASocket.hpp"
#include "requests/requests.hpp"
#include "configParsing/Parser.hpp"

class SocketClient : public ASocket
{
	protected:
		SocketClient();
		std::string	_header;
		std::string	_response;
	
	public:
	SocketClient(int port, int fd, const std::vector<ServerNode *> & ref);
	SocketClient(SocketClient const & other );
	virtual ~SocketClient();
	
	SocketClient & operator=(SocketClient const & other);
	
	void	addContent(char *content);
	void	clearAll();
	std::string	getResponse();
	void	setResponse(std::string input);

	//prepareResponse(iRequest *);
//	ServerNode *	_server;
//	const std::vector<ServerNode *> & _servers;
	iRequest *		_request; // allocation et suppression a chaque boucle de lecture/ecriture
};

#endif
