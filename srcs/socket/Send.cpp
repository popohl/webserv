/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/07 14:20:43 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"

void	serverLog(SocketClient & client)
{
	Rules rules;
	
	rules.setValues(*(client._request->findServer()), client._request->getRequestURI().c_str());
	std::cout << "\e[1;33m###########  LOG  ############\e[0m" << std::endl;
	std::cout << "\e[1;37mClient FD: \e[0m" << client.getSocketFd() << "\e[1;37m    Port: \e[0m" << client.getPort() << std::endl;
	std::cout << "\e[1;37mRequest  : \e[0m" << "\e[1;34m"<< client._request->printType() << "\e[0m"<< std::endl;
	std::cout << "\e[1;37mByte send: \e[0m" << client._totalSend << std::endl;
	//client._request->getRequestURI();
	std::cout << "\e[1;37mStatus   :\e[0m \e[1;32m" << client._responseStatus << "\e[0m" << std::endl; // mettre en couleur

	std::cout << "\e[1;33m######  SERVER RULES  ########\e[0m" << std::endl;
	std::cout << "\e[1;37mServer Name: \e[0m" << rules.serverName[0] <<std::endl;
	std::cout << "\e[1;37mAutodindex: \e[0m";
	if (rules.autoindex)
		std::cout << "\e[1;32mON\e[0m" << std::endl;
	else
		std::cout << "\e[1;31mOFF\e[0m" << std::endl;
	std::cout << "\e[1;37mCGI extension: \e[0m" << rules.cgiExtension <<std::endl;
	std::cout << "\e[1;37mCGI path: \e[0m" << rules.cgiPath <<std::endl;

	std::cout << "\e[1;33m##############################\e[0m" << std::endl;
}

void	sendToClient(ASocket *tmp_client, std::vector<ASocket*> & socket, t_FD & sets)
{
	SocketClient					*client = dynamic_cast<SocketClient*>(tmp_client);
	std::vector<char>		response = client->getResponse();
	int								ret;
	char							buffer[SENDING];

	if (response.size() > SENDING)
	{
		std::copy(response.begin(), response.begin() + SENDING, buffer);
		if ((ret = send(client->getSocketFd(), buffer, SENDING, 0)) < 0)
		{
			perror("Send failed:");
			deleteClient(*client, socket, sets);
			return ;
		}
		client->erasePartResponse(SENDING);
		client->_totalSend += ret;
	}
	else
	{
		std::copy(response.begin(), response.end(), buffer);
		if ((ret = send(client->getSocketFd(), buffer, response.size(), 0)) < 0)
		{
			perror("Send failed:");
			deleteClient(*client, socket, sets);
			return ;
		}
		client->_totalSend += ret;
		serverLog(*client);
		client->_totalSend = 0;
		delete client->_request;
		client->_request = NULL;
		client->clearAll();
		sets.writefds.remove(client->getSocketFd());
		sets.readfds.add(client->getSocketFd());
	}
}
