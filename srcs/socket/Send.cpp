/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/28 10:12:47 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

static int	contentSize(char *content)
{
	int i = 0;

	while (content[i])
		i++;
	return (i);
}

void	sendMessage(int link, char *buff)
{
	//TODO we have to analyse the message from the client and create the right content
	char	content[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	int		ret;
	int		temp;

	ret = 0;
	while (ret < contentSize(content)) //resend the data if not everything have been sent
	{
		if ((temp = send(link, &content[ret], contentSize(content), 0)) < 0)
		{
			perror("Send failed:");
			return ;
		}
		ret += temp;
		std::cout << " Message bytes =" << contentSize(content) << " Bytes Sent =" << ret << std::endl;
	}

	std::cout << "\n======== Message sent to client ========\n" << std::endl;
}
