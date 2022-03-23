/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 14:16:55 by fmonbeig         ###   ########.fr       */
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

void	sendToClient(ASocket *tmp_socket, std::vector<ASocket*> & socket, t_FD sets)
{
	SocketClient *client = dynamic_cast<SocketClient*>(tmp_socket);
	// std::string temp = client.getResponse();
	// const char *content = client.getResponse().c_str();
	int		ret;
	int		temp;

	char	content2[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	std::cout << "SEND socket number :" << client->getSocketFd() << std::endl;
	ret = 0;
	while (ret < contentSize(content2)) //resend the data if not everything have been sent
	{
		if ((temp = send(client->getSocketFd(), &content2[ret], contentSize(content2), 0)) < 0)
		{
			std::perror("Send failed:");
			return ;
		}
		ret += temp;
		std::cout << " Message bytes =" << contentSize(content2) << " Bytes Sent =" << ret << std::endl;
	}
	client->clearAll();
	std::cout << "\n======== Message sent to client ========\n" << std::endl;
	sets.writefds.remove(client->getSocketFd());
	sets.readfds.add(client->getSocketFd());
}
