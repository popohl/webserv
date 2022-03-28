/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileNode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 16:06:20 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 17:03:21 by pohl             ###   ########.fr       */
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

std::set<int>	ConfigFileNode::getListeningPorts( void ) const
{
	std::set<int>	listeningPorts;

	for (std::vector<ServerNode>::const_iterator it = serverList.begin();
			it != serverList.end(); it++)
		listeningPorts.insert(it->getServerRules().listenPort);
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
