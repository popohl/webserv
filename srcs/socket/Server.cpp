/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:50:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/17 17:35:36 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Server.hpp"

//TODO think to free socket at the end

static Socket*	createSocket(int port)
{
	int flag = 1;
	Socket *new_sock = new Socket(port);
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

static struct pollfd*	createPollfd(std::vector<Socket*> & socket) //create structure for poll() to listen at every socket
{
	struct pollfd *poll = new pollfd[socket.size()];
	for(int i = 0; i < socket.size(); i++)
	{
		poll[i].fd = socket[i]->getSocketFd();
		poll[i].events = POLLIN;  //TODO do we have to change POLLIN to POLLOUT when we are ready to send something to client ?
	}
	return (poll);
}

static void	portListening(std::vector<Socket*> & socket, struct pollfd* poll_fd)
{
	int	poll_ret;


	while (1)
	{
		std::cout << "\n----------- Waiting for new connection -----------\n" << std::endl;
		poll_ret = poll(poll_fd, socket.size(), -1); // with negative number poll never time out
		std::cout << "result of poll_ret : " << poll_ret << std::endl;

		if (poll_ret == 0)
			std::cout << " Server Time out" << std::endl; // related to the timeout parameter of poll
		else
		{
			for(int i = 0; poll_ret > 0; i++)
			{

				if (poll_fd[i].revents & POLLIN)
				{
					receiveConnectToClient(i, socket, poll_fd);
					poll_ret--;
				}
					if (poll_fd[i].revents & POLLOUT)
				{
					sendConnectToClient(i, socket, poll_fd); //Créer une classe fd client avec le buffer dedans, rajouter une interface ou une classe abstraite
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
	std::vector<Socket*>	socket;
	for(int i = 0; i < allPort.size(); i++)
		socket.push_back(createSocket(allPort[i]));

	//Create the struct of pollfd for function poll
	struct pollfd*	poll_fd;

	poll_fd = createPollfd(socket);
	portListening(socket, poll_fd);
}


//Pour que tout se passe bien il faut rajouter le fd du client dans la structure et il faut aussi pouvoir le remove quand il a fini
//Checker avec le flag POLLHUP permet de close le fd du client car il sait qu'il a été fermé ailleurs.
