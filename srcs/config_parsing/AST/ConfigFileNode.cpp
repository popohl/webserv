/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileNode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 16:06:20 by pohl              #+#    #+#             */
/*   Updated: 2022/03/14 16:01:50 by pohl             ###   ########.fr       */
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

void	ConfigFileNode::addServer( ServerNode new_server )
{
	serverList.push_back(new_server);
}

bool	ConfigFileNode::verbose = false;
