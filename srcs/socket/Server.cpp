/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:50:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/22 15:56:01 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//TODO think to free socket at the end

static ASocket*	createSocket(int port)
{
	int flag = 1;
	ASocket *new_sock = new SocketPort(port);
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

static void	portListening(std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd)
{
	int	poll_ret;

	while (1) // je peux garder en memoire les fd qui POLLHUP et les supprimer a chaque debut de boucle
	{
		std::cout << "\n----------- Waiting for new connection -----------\n" << std::endl;
		poll_ret = poll(poll_fd.data(), poll_fd.size(), 5000); // with negative number poll never time out
		std::cout << "result of poll_ret : " << poll_ret << std::endl;
		std::cout << "POLL_FD Size " << socket.size() << std::endl;
		std::cout << "Socket Size : " << poll_fd.size() << std::endl << std::endl;

		if (poll_ret == 0)
			std::cout << " Server Time out" << std::endl; // related to the timeout parameter of poll
		else
		{
			for (int i = 0; poll_ret > 0; i++)
			{
				if (poll_fd[i].revents & POLLHUP)
				{
					std::cout << "suppression of FD" << socket[i]->getSocketFd() << std::endl;
					removeFromPoll(socket, poll_fd, socket[i]->getSocketFd());
				}
				else if (poll_fd[i].revents & POLLIN)
				{
					std::cout << "CONNECT TO PORT "<< socket[i]->getSocketFd() << std::endl;
					connectToClient(i, socket, poll_fd);
					poll_ret--;
				}
				else if (poll_fd[i].revents & POLLOUT)
				{
					std::cout << "SEND TO Socket "<< socket[i]->getSocketFd() <<std::endl;
					sendToClient(*socket[i]);
					poll_ret--;
				}
			}
		}
	}
}

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

	//Create the extensible struct of pollfd for function poll
	std::vector<pollfd>	poll_fd;

	createPoll(socket, poll_fd);
	portListening(socket, poll_fd);
}


//Pour que tout se passe bien il faut rajouter le fd du client dans la structure et il faut aussi pouvoir le remove quand il a fini
//Checker avec le flag POLLHUP permet de close le fd du client car il sait qu'il a été fermé ailleurs.

// A common pattern is to:

// Use select/poll/epoll to detect when a socket can be read.
// Call recv once on a ready socket and append received data to a buffer.
// Check if the buffer contains enough data for processing. If yes, then process. Otherwise let select/poll/epoll tell when you can read more.


// Le but serait que les socket classique ne crée que les socketClient et les socket client font leur vie avec POLLIN et POLLOUT jusqu'a la mort

