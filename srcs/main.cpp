/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:11:42 by pohl              #+#    #+#             */
/*   Updated: 2022/04/06 16:04:33 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <signal.h>
#include "configParsing/Parser.hpp"
#include "cgi/Cgi.hpp"
#include "socket/ASocket.hpp"
#include "socket/Server.hpp"

void	free_memory(std::vector<ASocket*> & socket)
{
	for(size_t i = 0; i < socket.size(); i++)
		delete socket[i];
	throw std::exception();
	// throw std::exception("\e[0;31mSocket Failed in creation\n Exit webserv\e[0m");
}

int main( int argc, char **argv )
{
	if (argc == 2)
	{
		try
		{
			// Parser  parser("./srcs/testing/config_files/basic_file.conf");
			Parser			parser(argv[1]);
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
				temp = createSocket(serverIt->first, serverIt->second);
				if (temp)
					socket.push_back(temp);
				else
					free_memory(socket);
			}
	//		signal(SIGPIPE, SIG_IGN);
			//Create two sets of fd for select : readfds and writefds
			t_FD	sets;
			fillFdSets(sets, socket);
			portListening(sets, socket);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else
		std::cout << "usage: ./webserv path_to_config_file" << std::endl;
}
