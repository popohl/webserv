/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRules.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/20 19:03:45 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/AST/ServerRules.hpp"

ServerRules::ServerRules( void )
{
	if (ServerRules::verbose)
		std::cout << "Default constructor for ServerRules called" << std::endl;
	resetRules();
	return;
}

ServerRules::ServerRules( ServerRules const & src )
{
	if (ServerRules::verbose)
		std::cout << "Copy constructor for ServerRules called" << std::endl;
	*this = src;
	return;
}

ServerRules::~ServerRules( void )
{
	if (ServerRules::verbose)
		std::cout << "Destructor for ServerRules called" << std::endl;
	return;
}

ServerRules &	ServerRules::operator=( ServerRules const & src )
{
	if (ServerRules::verbose)
		std::cout << "Assignement operator for ServerRules called" << std::endl;
	if (this == &src)
		return *this;
	this->autoindex = src.autoindex;
	this->clientMaxBodySize = src.clientMaxBodySize;
	this->cgiExtension = src.cgiExtension;
	this->cgiPath = src.cgiPath;
	this->errorPage = src.errorPage;
	this->index = src.index;
	this->listenPort = src.listenPort;
	this->listenAddress = src.listenAddress;
	this->serverName = src.serverName;
	return *this;
}

void	ServerRules::resetRules( void )
{
	autoindex = false;
	clientMaxBodySize = 1048576;
	cgiExtension.clear();
	cgiPath.assign(1, "/cgi-bin/");
	errorPage.clear();
	index.clear();
	listenPort = 8000;
	listenAddress = "0.0.0.0";
	serverName.clear();
}

void	ServerRules::addErrorPage( int errorCode, std::string errorPagePath )
{
	this->errorPage[errorCode] = errorPagePath;
}

void	ServerRules::addIndex( std::string newIndex )
{
	this->index.push_back(newIndex);
}

void	ServerRules::addServerName( std::string newServerName )
{
	this->serverName.push_back(newServerName);
}

bool	ServerRules::verbose = false;
