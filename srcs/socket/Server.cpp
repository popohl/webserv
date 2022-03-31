/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:50:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/31 17:14:23 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket/Server.hpp"
#include "configParsing/Parser.hpp"

ASocket*	createSocket(int port, ServerNode * server) // change const reference to serverNode if possible
{
	int flag = 1;
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
		std::cout << "Connection to port: " << port << std::endl;
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

void	portListening(t_FD & sets, std::vector<ASocket*> & socket)
{
	fd_set			tmp_read;
	fd_set			tmp_write;
	int				ret;
	ASocket			*temp;
	SocketClient	*client;

	std::cout << "at the listening part, there is " << socket.size() << std::endl;
	while (1)
	{
		// We have to make a copy at each loop because select mess up the fd_set
		// We have to reinitialize timeval if we want to have a good Timeout because select change timeval
		fillTimeout(sets);
		tmp_read = sets.readfds.getset();
		tmp_write = sets.writefds.getset();
		fillFdMax(sets, socket);
		std::cout << "\n\e[0;35m----------- Waiting for New connection -----------\e[0m\n" << std::endl;
		if ((ret = select(sets.fdmax + 1, &tmp_read, &tmp_write, NULL, &sets.tv)) < 0)
			perror("Select:");
		if (ret == 0)
			std::cout << "Time Out" << std::endl;
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
				sendToClient(findSocket(i, socket), sets);
			}
			else
			{
				temp = findSocket(i, socket);
				if (temp && temp->getType() == CLIENT)
				{
					std::cout << "\e[4;31mYou take too long..\e[0m" << std::endl;
					client = dynamic_cast<SocketClient*>(temp);
					if (client->checkTimeout())
						deleteClient(*client, socket, sets);
				}
			}
		}
	}
}
/*
int main() //Mettre dans un fichier main.cpp
{
	// Get all the port to listen from Paul's Parsing
	std::vector<int>	allPort;
	allPort.push_back(8080);
	allPort.push_back(8003);
	allPort.push_back(8004);

	//Create a containers of Socket pointer. The Class Socket initialize the bind and the listening for every Socket
	std::vector<ASocket*>	socket;
	ASocket					*temp;
	for(int i = 0; i < allPort.size(); i++) // check le NULL de l'erreur
	{
		temp = createSocket(allPort[i]);
		if (temp)
			socket.push_back(temp);
	}
	//Create two sets of fd for select : readfds and writefds
	t_FD	sets;
	fillFdSets(sets, socket);
	portListening(sets, socket);
}
*/


// a faire :

// rajouter une image dans un HTML :: <img src="dinosaur.jpg"> (pour le poids je ne sais pas comment faire)
