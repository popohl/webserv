/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/05 16:41:16 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"

// static std::string	requestType(iRequest *request)
// {
// 	getRequest	*tempA = dynamic_cast<getRequest*>(request);
// 	if (tempA)
// 		return ("GET");
// 	else
// 	{
// 		postRequest	*tempB = dynamic_cast<postRequest*>(request);
// 		if (tempB)
// 			return ("POST");
// 		else
// 		{
// 			deleteRequest	*tempC = dynamic_cast<deleteRequest*>(request);
// 			if (tempC)
// 				return ("DELETE");
// 			else
// 			{
// 				return ("Undefined");
// 			}
// 		}
// 	}
// }

void	serverLog(SocketClient & client)
{
	ServerNode *server = client._request->findServer();
	ServerRules rules;

	if (server)
		rules = server->getServerRules();
	std::cout << "\e[1;33m###########  LOG  ############\e[0m" << std::endl;
	std::cout << "\e[1;37mClient FD: \e[0m" << client.getSocketFd() << "\e[1;37m    Port: \e[0m" << client.getPort() << std::endl;
	std::cout << "\e[1;37mRequest  : \e[0m" << "\e[1;34m"<< client._request->printType() << "\e[0m"<< std::endl;
	std::cout << "\e[1;37mByte send: \e[0m" << client._totalSend << std::endl;
	std::cout << "\e[1;37mStatus   :\e[0m \e[1;32mOK\e[0m" << std::endl;

	if (server)
	{
		std::cout << "\e[1;33m######  SERVER RULES  ########\e[0m" << std::endl;
		std::cout << "\e[1;37mServer Name: \e[0m" << rules.serverName[0] <<std::endl; //NOTA BENE pourquoi on a un vector de server name
		std::cout << "\e[1;37mAutodindex: \e[0m";
		if (rules.autoindex)
			std::cout << "\e[1;32mON\e[0m" << std::endl;
		else
			std::cout << "\e[1;31mOFF\e[0m" << std::endl;
		std::cout << "\e[1;37mCGI extension: \e[0m" << rules.cgiExtension <<std::endl;
		std::cout << "\e[1;37mCGI path: \e[0m" << rules.cgiPath <<std::endl;
	}
	std::cout << "\e[1;33m##############################\e[0m" << std::endl;
}

void	sendToClient(ASocket *tmp_socket, t_FD & sets)
{
	SocketClient	*client = dynamic_cast<SocketClient*>(tmp_socket);
	std::string		response = client->getResponse();
	int				ret;

	if (response.size() > SENDING)
	{
		if ((ret = send(client->getSocketFd(), response.c_str(), SENDING, 0)) < 0)
		{
			perror("Send failed:");
			return ;
		}
		client->erasePartResponse(SENDING);
		client->_totalSend += ret;
		// std::cout << "TOTAL = " << client->_totalSend << std::endl;
	}
	else
	{
		// std::cout << "***********" << std::endl << response << std::endl << "***********" << std::endl;
		if ((ret = send(client->getSocketFd(), response.c_str(), response.size(), 0)) < 0)
		{
			perror("Send failed:");
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
	// std::cout << " Message bytes =" << response.size() << " Bytes Sent =" << ret << std::endl;
	// std::cout << "\n\e[0;32m======== Message sent to client ========\e[0m\n" << std::endl;
	// Remove request to serve another one (need to handle multi write case)
}
