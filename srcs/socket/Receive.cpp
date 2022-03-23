/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 14:30:52 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void	receiveMessage(ASocket & tmp_socket, std::vector<ASocket*> & socket, t_FD sets)
{
	SocketClient	client = dynamic_cast<SocketClient&>(tmp_socket);
	int				ret;
	char			buff[90000];

	ret = 1;
	memset((void*)buff, 0, 90000);

	std::cout << "BEFORE RECV " << std::endl;
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
				socket.erase(socket.begin() + i);
				break;
			}
		}
		sets.readfds.remove(client.getSocketFd());
		close(client.getSocketFd());
		delete &client;
		return ;
	}
	std::cout << "value of recv "<< ret << std::endl;
	client.addContent(buff); //faire avec un append afin que ca soit plus propre
	//TODO Regarder si on a tout ce qu'il nous faut avant de mettre le client dans le write
	sets.readfds.remove(client.getSocketFd());
	sets.writefds.add(client.getSocketFd());
	// Multiple recv = multiple read when select tell us it is ok
}

void	createClient(ASocket & tmp_socket, std::vector<ASocket*> & socket, t_FD sets)
{
	int	temp_fd;
	SocketPort socket_port = dynamic_cast<SocketPort&>(tmp_socket);

	if ((temp_fd = accept(socket_port.getSocketFd(), (struct sockaddr *)&socket_port._address, (socklen_t*)&socket_port._addrlen))<0)
		std::perror("Accept failed:");

	// fcntl(temp_fd, F_SETFL, O_NONBLOCK);
	SocketClient *client = new SocketClient(socket_port.getPort(), temp_fd); // est ce qu on doit proteger les new ??
	socket.push_back(client);
	sets.readfds.add(temp_fd);
	std::cout << "CREATE socket number :" << client->getSocketFd() << std::endl;
}

void	receiveDataOrNewClient(int i, std::vector<ASocket*> & socket, t_FD sets)
{
	ASocket *tmp_socket;

	tmp_socket = findSocket(i, socket);
	if (tmp_socket->getType() == PORT)
		createClient(*tmp_socket, socket, sets);
	else if (tmp_socket->getType() == CLIENT)
		receiveMessage(*tmp_socket, socket, sets);
}


//For accept()ed socket in non-blocking mode you first try write()ing or
// send()ing data and only when they return EAGAIN or EWOULDBLOCK you
// wait for POLLOUT on that socket.
// Comme ca on ne va pas creer plein de port
