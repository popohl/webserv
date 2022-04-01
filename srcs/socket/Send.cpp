/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/01 16:16:55 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"

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
	}
	else
	{
		if ((ret = send(client->getSocketFd(), response.c_str(), response.size(), 0)) < 0)
		{
			perror("Send failed:");
			return ;
		}
		delete client->_request;
		client->_request = NULL;
		client->clearAll();
		sets.writefds.remove(client->getSocketFd());
		sets.readfds.add(client->getSocketFd());
	}
	std::cout << " Message bytes =" << response.size() << " Bytes Sent =" << ret << std::endl;
	std::cout << "\n\e[0;32m======== Message sent to client ========\e[0m\n" << std::endl;
	// Remove request to serve another one (need to handle multi write case)
}
