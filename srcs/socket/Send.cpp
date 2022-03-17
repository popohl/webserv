/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/17 17:13:52 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

static int	contentSize(char *content)
{
	int i = 0;

	while (content[i])
		i++;
	return (i);
}

void	sendMessage(int link, char *buff)
{
	//TODO we have to analyse the message from the client and create the right content
	// char	content[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	char content[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 202\n\n"
		"<form method=\"post\" enctype=\"multipart/form-data\"><div><label for=\"file\">select file</label><input type=\"file\" id=\"file\" name=\"file\" multiple></div><div><button>Envoyer</button></div></form>";

	int		ret;
	int		temp;

	ret = 0;
	while (ret < contentSize(content)) //resend the data if not everything have been sent
	{
		if ((temp = send(link, &content[ret], contentSize(content), 0)) < 0)
		{
			std::perror("Send failed:");
			return ;
		}
		ret += temp;
		std::cout << " Message bytes =" << contentSize(content) << " Bytes Sent =" << ret << std::endl;
	}

	std::cout << "\n======== Message sent to client ========\n" << std::endl;
}

void	sendConnectToClient(int i, std::vector<Socket*> & socket, struct pollfd* poll_fd)
{
	int	link;

	if ((link = accept(socket[i]->getSocketFd(), (struct sockaddr *)&socket[i]->_address, (socklen_t*)&socket[i]->_addrlen))<0)
		std::perror("Accept failed:");
	sendMessage(link, NULL);
	poll_fd[i].events = POLLIN;
	poll_fd[i].revents = 0;
	close(link);
}
