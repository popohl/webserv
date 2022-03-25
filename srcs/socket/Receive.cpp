/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/25 14:56:24 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void	receiveMessage(ASocket & tmp_socket, std::vector<ASocket*> & socket, t_FD & sets)
{
	SocketClient	client = dynamic_cast<SocketClient&>(tmp_socket);
	ASocket 		*addr;
	int				ret;
	char			buff[90000];

	ret = 1;
	memset((void*)buff, 0, 90000);
	if ((ret = recv(client.getSocketFd(), buff, 90000, 0)) < 0)
	{
		std::perror("Recv failed:");
		return ;
	}
	if (ret == 0) // if recv = 0 the connection is closed so we have to delete the client
	{
		for (int i = 0; i < socket.size(); i++)
		{
			if (client.getSocketFd() == socket[i]->getSocketFd())
			{
				addr = socket[i];
				socket.erase(socket.begin() + i);
				break;
			}
		}
		sets.readfds.remove(client.getSocketFd());
		close(client.getSocketFd());
		std::cout << "suppression of client " << client.getSocketFd() << ret << " ."<< std::endl;
		delete addr;
		return ;
	}
	std::cout << "value of recv "<< ret << std::endl << std::endl;
	client.addContent(buff);
	//TODO INSERER PARTIE DE PIERRE
	sets.readfds.remove(client.getSocketFd());
	sets.writefds.add(client.getSocketFd());
}

void	createClient(ASocket & tmp_socket, std::vector<ASocket*> & socket, t_FD & sets)
{
	int	temp_fd;
	SocketPort socket_port = dynamic_cast<SocketPort&>(tmp_socket);

	if ((temp_fd = accept(socket_port.getSocketFd(), (struct sockaddr *)&socket_port._address, (socklen_t*)&socket_port._addrlen))<0)
	{
		std::perror("Accept failed:");
		return ;
	}
	fcntl(temp_fd, F_SETFL, O_NONBLOCK);
	SocketClient *client = new SocketClient(socket_port.getPort(), temp_fd);
	socket.push_back(client);
	sets.readfds.add(temp_fd);
	std::cout << "New client is created // FD = " << client->getSocketFd() << std::endl;
}

void	receiveDataOrNewClient(int i, std::vector<ASocket*> & socket, t_FD & sets)
{
	ASocket *tmp_socket;

	tmp_socket = findSocket(i, socket);
	if (tmp_socket->getType() == PORT)
		createClient(*tmp_socket, socket, sets);
	else if (tmp_socket->getType() == CLIENT)
		receiveMessage(*tmp_socket, socket, sets);
}
