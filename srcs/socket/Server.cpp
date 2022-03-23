/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:50:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 12:09:22 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//TODO think to free socket at the end

static ASocket*	createSocket(int port)
{
	int flag = 1;
	SocketPort *new_sock = new SocketPort(port);
	fcntl(new_sock->getSocketFd(), F_SETFL, O_NONBLOCK);
	if (setsockopt(new_sock->getSocketFd(),SOL_SOCKET, SO_REUSEADDR, &flag, sizeof flag) == -1)
	{
		std::perror("setsockopt");
		exit(1);
	}
	new_sock->bindSocket();
	new_sock->listenSocket();
	return (new_sock);
}

static void	fillFdSets(t_FD sets, std::vector<ASocket*> & socket)
{
	sets.fdmax = 0;
	for (int i = 0; i < socket.size(); i++)
	{
		sets.readfds.add(socket[i]->getSocketFd());
		if (sets.fdmax < socket[i]->getSocketFd())
			sets.fdmax = socket[i]->getSocketFd();
	}
}

static ASocket *findSocket(int fd, std::vector<ASocket*> & socket)
{
	for (int i = 0; i < socket.size(); i++)
	{
		if ( i == socket[i]->getSocketFd())
			return (socket[i]);
	}
	return (NULL);
}

static void	portListening(t_FD sets, std::vector<ASocket*> & socket)
{
	fd_set	tmp_read;
	fd_set	tmp_write;
	int		ret;

	while (1)
	{
		// We have to make a copy a each loop because select mess up the fd_set
		tmp_read = sets.readfds.getset();
		tmp_write = sets.writefds.getset();
		std::cout << "\n----------- Waiting for new connection -----------\n" << std::endl;
		if ( (ret = select(sets.fdmax, &tmp_read, &tmp_write, NULL, NULL)) < 0)
			std::perror("Select:");
		for (int i = 0; i < sets.fdmax && ret; i++)
		{
			if (FD_ISSET(i, &tmp_read))
			{
				ret--;
				receiveDataOrNewClient(i, socket, sets);
			}
			else if (FD_ISSET(i, &tmp_write))
			{
				ret--;
				findSocket(i, socket);
			}
		}
	}
}

// select
// int select(int numfds, fd_set *readfds, fd_set *writefds,
//            fd_set *exceptfds, struct timeval *timeout);
// Pour select il faut utiliser des set de FD que l'on va manipuler avec des Macros
// FD_SET(int fd, fd_set *set);	Add fd to the set.
// FD_CLR(int fd, fd_set *set);	Remove fd from the set.
// FD_ISSET(int fd, fd_set *set);	Return true if fd is in the set.
// FD_ZERO(fd_set *set);	Clear all entries from the set.
//
// Mettre timeval a NULL met le TIMEOUT a -1 (en gros)
// on check les fd a lire dans Readfds et ensuite on check ceux a ecrire dans writefd
// Retourne comme POLL
//
//
// What happens if a socket in the read set closes the connection? Well, in that case, select() returns with that socket descriptor set as “ready to read”. When you actually do recv() from it, recv() will return 0.
// That’s how you know the client has closed the connection.

// fdmax = last socket
// On peut creer une class select qui va faire la fonction la select et attendre la reponse
// et on peut avoir dans cette structure les diffents fd set et des fonctions afin de bien l utiliser
// on peut avoir la variable qui keep in track le fd max
// On doit avoir un fd set master avec tous les elements a l interieur et on doit le faire evoluer au fur et a mesure et le copier au dernier moment dans Readfd


int main()
{
	// Get all the port to listen from Paul's Parsing
	std::vector<int>	allPort;
	allPort.push_back(8080);
	// allPort.push_back(8046);
	// allPort.push_back(9056);

	//Create a containers of Socket pointer. The Class Socket initialize the bind and the listening for every Socket
	std::vector<ASocket*>	socket;
	for(int i = 0; i < allPort.size(); i++)
		socket.push_back(createSocket(allPort[i]));

	//Create two sets of fd for select : readfds and writefds
	t_FD	sets;
	fillFdSets(sets, socket);
	portListening(sets, socket);
}


//Pour que tout se passe bien il faut rajouter le fd du client dans la structure et il faut aussi pouvoir le remove quand il a fini
//Checker avec le flag POLLHUP permet de close le fd du client car il sait qu'il a été fermé ailleurs.

// A common pattern is to:

// Use select/poll/epoll to detect when a socket can be read.
// Call recv once on a ready socket and append received data to a buffer.
// Check if the buffer contains enough data for processing. If yes, then process. Otherwise let select/poll/epoll tell when you can read more.


// Le but serait que les socket classique ne crée que les socketClient et les socket client font leur vie avec POLLIN et POLLOUT jusqu'a la mort


// select
// int select(int numfds, fd_set *readfds, fd_set *writefds,
//            fd_set *exceptfds, struct timeval *timeout);
// Pour select il faut utiliser des set de FD que l'on va manipuler avec des Macros
// FD_SET(int fd, fd_set *set);	Add fd to the set.
// FD_CLR(int fd, fd_set *set);	Remove fd from the set.
// FD_ISSET(int fd, fd_set *set);	Return true if fd is in the set.
// FD_ZERO(fd_set *set);	Clear all entries from the set.
//
// Metttre timeval a NULL met le TIMEOUT a -1 (en gros)
// on check les fd a lire dans Readfds et ensuite on check ceux a ecrire dans writefd
// Retourne comme POLL
//
//
// What happens if a socket in the read set closes the connection? Well, in that case, select() returns with that socket descriptor set as “ready to read”. When you actually do recv() from it, recv() will return 0.
// That’s how you know the client has closed the connection.

// fdmax = last socket
// On peut creer une structure select qui va faire la fonction la select et attendre la reponse
// et on peut avoir dans cette structure les diffents fd set et des fonctions afin de bien l utiliser
// on peut avoir la variable qui keep in track le fd max
// On doit avoir un fd set master avec tous les elements a l interieur et on doit le faire evoluer au fur et a mesure et le copier au dernier moment dans Readfd


//Select will change the inside of the set so it's unusable after, we have to copy it at each round
