/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/07 14:18:13 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"
#include "requests/requests.hpp"

void	deleteClient(SocketClient & client, std::vector<ASocket*> & socket, t_FD & sets)
{
	ASocket 		*addr = NULL;

	for (size_t i = 0; i < socket.size(); i++)
	{
		if (client.getSocketFd() == socket[i]->getSocketFd())
		{
			addr = socket[i];
			socket.erase(socket.begin() + i);
			break;
		}
	}
	sets.readfds.remove(client.getSocketFd());
	sets.writefds.remove(client.getSocketFd());
	close(client.getSocketFd());
	delete addr;
}

std::vector<char> buildSendReponse(SocketClient & client)
{
	std::vector<char> responseRawData;
	response response = client._request->createResponse();
	client._responseStatus = response.getStatus();
	responseRawData = response.createFormattedResponse();
	return (responseRawData);
}

static void	receiveMessage(ASocket & tmp_socket, std::vector<ASocket*> & socket, t_FD & sets)
{
	SocketClient	&client = dynamic_cast<SocketClient&>(tmp_socket);
	int				ret;
	char			buff[90000];

	ret = 1;
	memset((void*)buff, 0, 90000);
	if ((ret = recv(client.getSocketFd(), buff, 90000, 0)) < 0)
	{
		perror("Recv failed:");
		deleteClient(client, socket, sets);
		return ;
	}
	if (ret == 0) // if recv = 0 the connection is closed so we have to delete the client
	{
		deleteClient(client, socket, sets);
		return ;
	}
	std::vector<char> data;
	data.reserve(ret);
	data.assign(buff, buff + ret);
	if (!client._request)
		client._request = iRequest::createRequest(data, client._servers);
	else
		client._request->_message.parseRequest(data);

	if (client._request->receivingisDone())
	{
		client.setResponse(buildSendReponse(client));
		sets.readfds.remove(client.getSocketFd());
		sets.writefds.add(client.getSocketFd());
		client.resetTimer();
	}
}

void	createClient(ASocket & tmp_socket, std::vector<ASocket*> & socket, t_FD & sets)
{
	int	temp_fd;
	SocketPort socket_port = dynamic_cast<SocketPort&>(tmp_socket);

	if ((temp_fd = accept(socket_port.getSocketFd(), (struct sockaddr *)&socket_port._address, (socklen_t*)&socket_port._addrlen))<0)
	{
		perror("Accept failed:");
		return ;
	}
	fcntl(temp_fd, F_SETFL, O_NONBLOCK);
	SocketClient *client = new SocketClient(socket_port.getPort(), temp_fd, socket_port._servers);
	socket.push_back(client);
	sets.readfds.add(temp_fd);
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
