/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:11:42 by pohl              #+#    #+#             */
/*   Updated: 2022/03/31 11:06:30 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "configParsing/Parser.hpp"
#include "socket/ASocket.hpp"
#include "socket/Server.hpp"

int main( int argc, char **argv )
{
	(void)argc;
	(void)argc;(void)argv;
	//protect for one argument usecase
//	Parser			parser(argv[1]);
	Parser  parser("./srcs/testing/config_files/basic_file.conf");
	ConfigFileNode	config = parser.getConfigFile();

	// Get all the port to listen from configuration file parsing
	mapPortToServers listeningPorts = config.getListeningPorts();

	//Create a containers of Socket pointer.
	//The Class Socket initialize the bind and the listening for every Socket
	std::vector<ASocket*>	socket;
	ASocket					*temp;
	for (mapPortToServers::iterator serverIt = listeningPorts.begin();
		 serverIt != listeningPorts.end();
		 serverIt++)
	{
		temp = createSocket(serverIt->first, serverIt->second[0]);
		if (temp)
			socket.push_back(temp);
			// TODO If NULL est ce qu'on kill le server ?
	}
	//Create two sets of fd for select : readfds and writefds
	t_FD	sets;
	fillFdSets(sets, socket);
	portListening(sets, socket);
}
