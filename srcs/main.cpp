/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:11:42 by pohl              #+#    #+#             */
//   Updated: 2022/04/06 13:29:31 by pcharton         ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <signal.h>
#include "configParsing/Parser.hpp"
#include "socket/ASocket.hpp"
#include "socket/Server.hpp"

void	free_memory(std::vector<ASocket*> & socket)
{
	for(size_t i = 0; i < socket.size(); i++)
		delete socket[i];
	std::exit(1);
}

int main( int argc, char **argv )
{
	if (argc == 2)
	{
		Parser  parser(argv[1]);
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
				free_memory(socket); //NOTA BENE fonction qui free tout si une socket ne se Fabrique/Bind/Listen pas bien
		}
		signal(SIGPIPE, SIG_IGN);
		//Create two sets of fd for select : readfds and writefds
		t_FD	sets;
		fillFdSets(sets, socket);
		portListening(sets, socket);
	}
	else
		std::cout << "usage: ./webserv path_to_config_file" << std::endl; 
}

//TODO faire un logger propre avec les infos presente dans irequest
//TODO faire un fichier qui va autoindex les informations d'une page
//TODO Les liens vers les dossiers vont aussi autoindex des pages
