/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:50:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/16 15:15:11 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Server.hpp"

//TODO think to free socket at the end

static Socket*	createSocket(int port)
{
	Socket *new_sock = new Socket(port);
	fcntl(new_sock->getSocketFd(), F_SETFL, O_NONBLOCK);
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

static void	receiveMessage(int link)
{
	char	buff[90000];
	std::string	temp;
	int		ret;

	memset((void*)buff, 0, 90000);
	if ((ret = recv(link, buff, 90000, 0)) < 0)
	{
		std::perror("Recv failed:");
		return ;
	}
	temp = buff;
	std::cout << temp << std::endl;
	sendMessage(link, buff);
}

static void	portListening(std::vector<Socket*> & socket, struct pollfd* poll_fd)
{
	int	poll_ret;
	int	link;

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
				if (poll_fd[i].revents & POLLIN) //TODO we have to listen the read And the write, but why ?
				{
					poll_ret--;
					if ((link = accept(socket[i]->getSocketFd(), (struct sockaddr *)&socket[i]->_address, (socklen_t*)&socket[i]->_addrlen))<0)
						std::perror("Accept failed:");
					receiveMessage(link);
					close(link);
				}
			}
		}
	}
}

int main()
{
	// Get all the port to listen from Paul's Parsing
	std::vector<int>	allPort;
	allPort.push_back(8082);
	allPort.push_back(8042);
	allPort.push_back(9052);

	//Create a containers of Socket pointer. The Class Socket initialize the bind and the listening for every Socket
	std::vector<Socket*>	socket;
	for(int i = 0; i < allPort.size(); i++)
		socket.push_back(createSocket(allPort[i]));

	//Create the struct of pollfd for function poll
	struct pollfd*	poll_fd;

	poll_fd = createPollfd(socket);
	portListening(socket, poll_fd);
}


//TODO faire des tests avec google test

//Cookie :
