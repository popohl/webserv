/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRules.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/24 17:00:50 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/AST/ServerRules.hpp"

ServerRules::ServerRules( void )
{
	resetRules();
	return;
}

ServerRules::ServerRules( ServerRules const & src )
{
	*this = src;
	return;
}

ServerRules::~ServerRules( void )
{
	return;
}

ServerRules &	ServerRules::operator=( ServerRules const & src )
{
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
	cgiPath.clear();
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

void	ServerRules::addServerName( std::string newServerName )
{
	this->serverName.push_back(newServerName);
}
