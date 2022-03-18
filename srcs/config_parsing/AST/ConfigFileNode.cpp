/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileNode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 16:06:20 by pohl              #+#    #+#             */
/*   Updated: 2022/03/18 11:50:29 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parsing/AST/ConfigFileNode.hpp"

ConfigFileNode::ConfigFileNode( void )
{
	if (ConfigFileNode::verbose)
		std::cout << "Default constructor for ConfigFileNode called" << std::endl;
	serverList.clear();
	return;
}

ConfigFileNode::ConfigFileNode( ConfigFileNode const & src )
{
	if (ConfigFileNode::verbose)
		std::cout << "Copy constructor for ConfigFileNode called" << std::endl;
	*this = src;
	return;
}

ConfigFileNode::~ConfigFileNode( void )
{
	if (ConfigFileNode::verbose)
		std::cout << "Destructor for ConfigFileNode called" << std::endl;
	return;
}

ConfigFileNode &	ConfigFileNode::operator=( ConfigFileNode const & src )
{
	if (ConfigFileNode::verbose)
		std::cout << "Assignement operator for ConfigFileNode called" << std::endl;
	if (this == &src)
		return *this;
	this->serverList = src.serverList;
	return *this;
}

std::vector<ServerNode>			&ConfigFileNode::getServerList( void )
{
	return serverList;
}

const std::vector<ServerNode>	&ConfigFileNode::getServerList( void ) const
{
	return serverList;
}

std::set<int>	ConfigFileNode::getListeningPorts( void ) const
{
	std::set<int>	listeningPorts;

	for (std::vector<ServerNode>::const_iterator it = serverList.begin();
			it != serverList.end(); it++)
		listeningPorts.insert(it->getServerRules().listen_port);
	return listeningPorts;
}

ServerNode	&ConfigFileNode::LatestServer( void )
{
	return serverList.back();
}

ServerNode&	ConfigFileNode::createNewServerNode( void )
{
	ServerNode	newServer;

	serverList.push_back(newServer);
	return serverList.back();
}

bool	ConfigFileNode::verbose = false;
