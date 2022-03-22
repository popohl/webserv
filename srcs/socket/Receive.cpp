/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/22 15:21:48 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void	receiveMessage(SocketClient & link)
{
	std::string	temp;
	int			ret;
	char		buff[90000];

	ret = 1;
	memset((void*)buff, 0, 90000);
	// while(ret < 8000)
	// {
		std::cout << "BEFORE RECV " << std::endl;
		if ((ret = recv(link.getSocketFd(), buff, 90000, 0)) < 0)
		{
			std::perror("Recv failed:");
			return ;
		}
		std::cout << "value of recv "<< ret << std::endl;
		link.addContent(buff);
	// }
	//  sendToClient(link);
}

void	connectToClient(int i, std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd)
{
	int	temp_fd;

	if ((temp_fd = accept(socket[i]->getSocketFd(), (struct sockaddr *)&socket[i]->_address, (socklen_t*)&socket[i]->_addrlen))<0)
		std::perror("Accept failed:");

	// fcntl(temp_fd, F_SETFL, O_NONBLOCK);
	SocketClient *link = new SocketClient(socket[i]->getPort(), temp_fd); // est ce qu on doit proteger les new ??
	receiveMessage(*link);
	socket.push_back(link);
	addToPoll(*link, poll_fd);
	std::cout << "CREATE socket number :" << link->getSocketFd() << std::endl;
}

//For accept()ed socket in non-blocking mode you first try write()ing or
// send()ing data and only when they return EAGAIN or EWOULDBLOCK you
// wait for POLLOUT on that socket.
// Comme ca on ne va pas creer plein de port
