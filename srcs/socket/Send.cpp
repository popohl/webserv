/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:58:15 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/21 17:57:34 by fmonbeig         ###   ########.fr       */
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

	const char *content = link.getResponse().c_str();
	int		ret;
	int		temp;

	ret = 0;
	while (ret < contentSize(content)) //resend the data if not everything have been sent
	{
		if ((temp = send(link.getSocketFd(), &content[ret], contentSize(content), 0)) < 0)
		{
			std::perror("Send failed:");
			return ;
		}
		ret += temp;
		std::cout << " Message bytes =" << contentSize(content) << " Bytes Sent =" << ret << std::endl;
	}
	std::cout << "\n======== Message sent to client ========\n" << std::endl;

	close(link.getSocketFd());
}
