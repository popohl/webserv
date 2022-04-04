/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/04 19:42:06 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"

void	sendToClient(ASocket *tmp_socket, t_FD & sets)
{
	SocketClient	*client = dynamic_cast<SocketClient*>(tmp_socket);
	std::vector<unsigned char>		response = client->getResponse();
	int				ret;
	char			buffer[SENDING];

	if (response.size() > SENDING)
	{
		std::copy(response.begin(), response.begin() + SENDING, buffer);
		if ((ret = send(client->getSocketFd(), buffer, SENDING, 0)) < 0)
		{
			perror("Send failed:");
			return ;
		}
		client->erasePartResponse(SENDING);
	}
	else
	{
		std::copy(response.begin(), response.end(), buffer);
		if ((ret = send(client->getSocketFd(), buffer, response.size(), 0)) < 0)
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
//	std::cout << " Message bytes =" << response.size() << " Bytes Sent =" << ret << std::endl;
//	std::cout << "\n\e[0;32m======== Message sent to client ========\e[0m\n" << std::endl;
	// Remove request to serve another one (need to handle multi write case)
}
