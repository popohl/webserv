/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 12:11:16 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define PORT 1
#define CLIENT 2

#include <iostream>
#include <cstring>
#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <iterator>

#include "ASocket.hpp"
#include "SocketClient.hpp"
#include "SocketPort.hpp"
#include "FdSet.hpp"

typedef struct s_FD
{
	int	fdmax;
	FdSet readfds;
	FdSet writefds;
}	t_FD;

void	sendMessage(int link, char *buff);
void	receiveDataOrNewClient(int i, std::vector<ASocket*> & socket, t_FD sets);
void	sendToClient(ASocket  & link);
static ASocket *findSocket(int fd, std::vector<ASocket*> & socket);
//FD management
void	createPoll(std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd);
void	addToPoll(ASocket & socket, std::vector<pollfd> & poll_fd);
void	removeFromPoll(std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd, int fd);

#endif
