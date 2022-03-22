/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/22 14:43:06 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static int	contentSize(const char *content)
{
	int i = 0;

	while (content[i])
		i++;
	return (i);
}

void	sendToClient(ASocket & temp_link)
{
	SocketClient link = dynamic_cast<SocketClient&>(temp_link);

	// std::string temp = link.getResponse();
	// const char *content = link.getResponse().c_str();
	int		ret;
	int		temp;

	char	content2[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	std::cout << "SEND socket number :" << link.getSocketFd() << std::endl;
	ret = 0;
	while (ret < contentSize(content2)) //resend the data if not everything have been sent
	{
		if ((temp = send(link.getSocketFd(), &content2[ret], contentSize(content2), 0)) < 0)
		{
			std::perror("Send failed:");
			return ;
		}
		ret += temp;
		std::cout << " Message bytes =" << contentSize(content2) << " Bytes Sent =" << ret << std::endl;
	}
	std::cout << "\n======== Message sent to client ========\n" << std::endl;

	// close(link.getSocketFd());
}
