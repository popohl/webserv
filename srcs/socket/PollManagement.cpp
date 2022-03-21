/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManagement.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 14:40:35 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 17:27:51 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	createPoll(std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd) //create structure for poll() to listen at every socket
{
	struct pollfd	poll;
	for (int i = 0; i < socket.size(); i++)
	{
		poll.fd = socket[i]->getSocketFd();
		poll.events = POLLIN;
		poll_fd.push_back(poll);
	}
}

void	addToPoll(ASocket & socket, std::vector<pollfd> & poll_fd)
{
	struct pollfd	poll;

	poll.fd = socket.getSocketFd();
	poll.events = POLLOUT;
	poll_fd.push_back(poll);
}

void	removeFromPoll(std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd, int fd)
{
	for (int i = 0; i < socket.size(); i++)
	{
		if (socket[i]->getSocketFd() == fd)
		{
			socket.erase(socket.begin() + i);
			break ;
		}
	}

	for (int i = 0; i < poll_fd.size(); i++)
	{
		if (poll_fd[i].fd == fd)
		{
			socket.erase(socket.begin() + i);
			break ;
		}
	}

}
