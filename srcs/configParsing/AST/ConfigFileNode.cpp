/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileNode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 16:06:20 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 14:13:08 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/AST/ConfigFileNode.hpp"

ConfigFileNode::ConfigFileNode( void )
{
	serverList.clear();
	return;
}

ConfigFileNode::ConfigFileNode( ConfigFileNode const & src )
{
	*this = src;
	return;
}

ConfigFileNode::~ConfigFileNode( void )
{
	return;
}

ConfigFileNode &	ConfigFileNode::operator=( ConfigFileNode const & src )
{
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

mapPortToServers	ConfigFileNode::getListeningPorts( void )
{
	mapPortToServers	listeningPorts;

	for (std::vector<ServerNode>::iterator it = serverList.begin();
			it != serverList.end(); it++)
		listeningPorts[it->getServerRules().listenPort].push_back(&(*it));
	return listeningPorts;
}

ServerNode	&ConfigFileNode::latestServer( void )
{
	return serverList.back();
}

ServerNode&	ConfigFileNode::createNewServerNode( void )
{
	ServerNode	newServer;

	serverList.push_back(newServer);
	return serverList.back();
}
