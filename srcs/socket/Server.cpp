/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:50:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/07 17:04:35 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"
#include "configParsing/Parser.hpp"

ASocket*	createSocket(int port, const std::vector<ServerNode *> & server) // change const reference to serverNode if possible
{
	int	flag = 1;
	try
	{
		SocketPort *new_sock = new SocketPort(port, server);
		fcntl(new_sock->getSocketFd(), F_SETFL, O_NONBLOCK);
		if (setsockopt(new_sock->getSocketFd(),SOL_SOCKET, SO_REUSEADDR, &flag, sizeof flag) == -1)
			perror("setsockopt");
		if (!new_sock->bindSocket())
		{
			close(new_sock->getSocketFd());
			delete new_sock;
			return (NULL);
		}
		if (!new_sock->listenSocket())
		{
			close(new_sock->getSocketFd());
			delete new_sock;
			return (NULL);
		}
		std::cout << "Port listening: "<<"\e[0;32m"<< port <<"\e[0m"<< std::endl;
		return (new_sock);
	}
		catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (NULL);
	}
}

void	fillFdSets(t_FD & sets, std::vector<ASocket*> & socket)
{
	sets.fdmax = 0;
	for (size_t i = 0; i < socket.size(); i++)
	{
		sets.readfds.add(socket[i]->getSocketFd());
	}
}

static void	fillTimeout(t_FD & sets)
{
	sets.tv.tv_sec = 60;
	sets.tv.tv_usec = 500000;
}

static void	fillFdMax(t_FD & sets, std::vector<ASocket*> & socket)
{
	sets.fdmax = 0;

	for (size_t i = 0; i < socket.size(); i++)
	{
		if (sets.fdmax < socket[i]->getSocketFd())
			sets.fdmax = socket[i]->getSocketFd();
	}
}

ASocket *findSocket(int fd, std::vector<ASocket*> & socket)
{
	for (size_t i = 0; i < socket.size(); i++)
	{
		if ( fd == socket[i]->getSocketFd())
			return (socket[i]);
	}
	return (NULL);
}

// We have to make a copy of fdsets at each loop because select mess up the fd inside
// We have to reinitialize timeval if we want to have a good Client Timeout because select change timeval

void	portListening(t_FD & sets, std::vector<ASocket*> & socket)
{
	fd_set			tmp_read;
	fd_set			tmp_write;
	int				ret;
	ASocket			*temp;
	SocketClient	*client;

	while (1)
	{
		fillTimeout(sets);
		tmp_read = sets.readfds.getset();
		tmp_write = sets.writefds.getset();
		fillFdMax(sets, socket);
		if ((ret = select(sets.fdmax + 1, &tmp_read, &tmp_write, NULL, &sets.tv)) < 0)
			perror("Select:");
		if (ret == 0)
			std::cout << "Server Time out" << std::endl;
		for (int i = 0; i <= sets.fdmax && ret; i++)
		{
			if (FD_ISSET(i, &tmp_read))
			{
				ret--;
				receiveDataOrNewClient(i, socket, sets);
			}
			else if (FD_ISSET(i, &tmp_write))
			{
				ret--;
				sendToClient(findSocket(i, socket), socket, sets);
			}
			else
			{
				temp = findSocket(i, socket);
				if (temp && temp->getType() == CLIENT)
				{
					client = dynamic_cast<SocketClient*>(temp);
					if (client->checkTimeout())
					{
						std::cout << "\e[0;31mClient " << client->getSocketFd() <<"Time out\e[0m" << std::endl;
						deleteClient(*client, socket, sets);
					}
				}
			}
		}
	}
}
