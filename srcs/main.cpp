/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:11:42 by pohl              #+#    #+#             */
//   Updated: 2022/03/28 14:03:13 by pcharton         ###   ########.fr       //
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
//	Parser			parser("./srcs/testing/config_files/everyRight.conf");
//	ConfigFileNode	result = parser.getConfigFile();
//	Parser			parser(argv[1]);
//	ConfigFileNode	result = parser.getConfigFile();


	// Get all the port to listen from Paul's Parsing
	std::vector<int>	allPort;
	allPort.push_back(8080);
	allPort.push_back(8003);
	allPort.push_back(8004);

	//Create a containers of Socket pointer. The Class Socket initialize the bind and the listening for every Socket
	std::vector<ASocket*>	socket;
	ASocket					*temp;
	for(size_t i = 0; i < allPort.size(); i++) // check le NULL de l'erreur
	{
		temp = createSocket(allPort[i]);
		if (temp)
			socket.push_back(temp);
	}
	//Create two sets of fd for select : readfds and writefds
	t_FD	sets;
	fillFdSets(sets, socket);
	portListening(sets, socket);
}
