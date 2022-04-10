/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:11:42 by pohl              #+#    #+#             */
//   Updated: 2022/04/08 19:13:28 by pcharton         ###   ########.fr       //
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
}

	//1- Parse the config file and get all the port to listen
	//2 -Create a containers of Socket pointer.
	//3 -With Socket Class you can bind and listening a new Socket easily
	//4 -Create two sets of fd for select : readfds and writefds
	//5- Mainloop of the server with select

int main( int argc, char **argv )
{
	if (argc > 2)
	{
		std::cout << "usage: ./webserv [path_to_config_file]" << std::endl;
		exit(1);
	}
	try
	{
		Parser			parser(argc == 2 ? argv[1] : "./config_files/config.conf");
		ConfigFileNode	config = parser.getConfigFile();
		mapPortToServers listeningPorts = config.getListeningPorts();
		std::vector<ASocket*>	socket;
		ASocket					*temp;

		std::cout << "\e[0;35m---WELCOME HUMAN---\e[0m" << std::endl;
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
		signal(SIGPIPE, SIG_IGN);
		t_FD	sets;
		fillFdSets(sets, socket);
		portListening(sets, socket);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
