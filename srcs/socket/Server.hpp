/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:54:51 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 17:48:25 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define TIMEOUT -1

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

void	sendMessage(int link, char *buff);
void	connectToClient(int i, std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd);
void	sendToClient(ASocket  & link);
//FD management
void	createPoll(std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd);
void	addToPoll(ASocket & socket, std::vector<pollfd> & poll_fd);
void	removeFromPoll(std::vector<ASocket*> & socket, std::vector<pollfd> & poll_fd, int fd);


#endif
