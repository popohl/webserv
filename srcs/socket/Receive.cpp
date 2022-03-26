/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
//   Updated: 2022/03/26 14:50:38 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"
#include "requests/requests.hpp"

static void	receiveMessage(ASocket & tmp_socket, std::vector<ASocket*> & socket, t_FD & sets)
{
	SocketClient	&client = dynamic_cast<SocketClient&>(tmp_socket);
	ASocket 		*addr = NULL;
	int				ret;
	char			buff[90000];

	ret = 1;
	memset((void*)buff, 0, 90000);
	if ((ret = recv(client.getSocketFd(), buff, 90000, 0)) < 0)
	{
		perror("Recv failed:");
		return ;
	}
	if (ret == 0) // if recv = 0 the connection is closed so we have to delete the client
	{
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
		close(client.getSocketFd());
		std::cout << "suppression of client " << client.getSocketFd() << ret << " ."<< std::endl;
		delete addr;
		return ;
	}
	std::cout << "value of recv "<< ret << std::endl << std::endl;
	client.addContent(buff);
	std::string tmp(buff);
	//TODO INSERER PARTIE DE PIERRE
	if (!client._request)
		client._request = iRequest::createRequest(tmp);
//	std::cout << client._request << std::endl;
//	std::cout << "after creating request : tmp content is : " << tmp << std::endl;
//	if iRequest fails, send 405 Method Not Allowed
//	delete (client._request);
//	client._request = NULL;

	if (client._request->receivingisDone())
	{
		std::cout << "when receving is done " << &client << " "<< client._request << std::endl;
		client.setResponse(client._request->createResponse());
		std::cout << "client fd is : " << client.getSocketFd() << std::endl;
		std::cout << "formulated response is " << client.getResponse() << std::endl;
		sets.readfds.remove(client.getSocketFd());
		sets.writefds.add(client.getSocketFd());
	}
/****
	 * if (!ptr) // pas de requete cree
	 * 	client._request = createRequest();
	 * if (!client._request->headerIsComplete()
	 * 	|| !client._request->bodyIsComplete())
	 * 		return ;
	 * else
	 * 	{
	 * 		sets.readfds.remove(client.getSocketFd());
			sets.writefds.add(client.getSocketFd());
	 * 	}
	 * */
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
