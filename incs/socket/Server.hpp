/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/04/06 15:30:50 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define PORT 1
#define CLIENT 2
#define SENDING 1000

#include <iostream>
#include <cstring>
#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <iterator>
#include <time.h>
//#include <cstdbool>
#include <stdexcept>
#include <cstdlib>
#include "ASocket.hpp"
#include "SocketClient.hpp"
#include "SocketPort.hpp"
#include "FdSet.hpp"

typedef struct s_FD
{
	struct timeval tv;
	int	fdmax;
	FdSet readfds;
	FdSet writefds;
}	t_FD;

void		sendMessage(int link, char *buff);
void		receiveDataOrNewClient(int i, std::vector<ASocket*> & socket, t_FD & sets);
void		sendToClient(ASocket *tmp_client, std::vector<ASocket*> & socket, t_FD & sets);
void		deleteClient(SocketClient & client, std::vector<ASocket*> & socket, t_FD & sets);

ASocket*	createSocket(int port, const std::vector<ServerNode *> & server);
ASocket		*findSocket(int fd, std::vector<ASocket*> & socket);

void		fillFdSets(t_FD & sets, std::vector<ASocket*> & socket);
void		portListening(t_FD & sets, std::vector<ASocket*> & socket);
std::string	autoIndex(std::string root);


#endif
