/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/17 16:14:01 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Server.hpp"

static void	receiveMessage(int link)
{
	char	buff[90000];
	std::string	temp;
	int		ret;

	ret = 1;
	memset((void*)buff, 0, 90000);

	// while(ret < 8000)
	// {
		if ((ret = recv(link, buff, 90000, 0)) < 0)
		{
			std::perror("Recv failed:");
			return ;
		}
		std::cout << "value of recv "<< ret << std::endl;
		temp = buff;
		std::cout << temp << std::endl;
	// }
	sendMessage(link, buff);
}

void	receiveConnectToClient(int i, std::vector<Socket*> & socket, struct pollfd* poll_fd)
{
	int	link;

	if ((link = accept(socket[i]->getSocketFd(), (struct sockaddr *)&socket[i]->_address, (socklen_t*)&socket[i]->_addrlen))<0)
		std::perror("Accept failed:");
	receiveMessage(link);
	close(link);
}
