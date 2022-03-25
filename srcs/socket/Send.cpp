/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/25 15:26:35 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static int	contentSize(const char *content)
{
	int i = 0;

	while (content[i])
		i++;
	return (i);
}

void	sendToClient(ASocket *tmp_socket, std::vector<ASocket*> & socket, t_FD & sets)
{
	SocketClient	*client = dynamic_cast<SocketClient*>(tmp_socket);
	std::string		response = client->getResponse();
	int				ret;
	int				total;

	total = 0;
	while (total < response.size())
	{
		if ((ret = send(client->getSocketFd(), response.c_str() + total, response.size(), 0)) < 0)
		{
			std::perror("Send failed:");
			return ;
		}
		total += ret;
		std::cout << " Message bytes =" << response.size() << " Bytes Sent =" << ret << std::endl;
		std::cout << "\n======== Message sent to client ========\n" << std::endl;
	}
	client->clearAll();
	sets.writefds.remove(client->getSocketFd());
	sets.readfds.add(client->getSocketFd());
}
