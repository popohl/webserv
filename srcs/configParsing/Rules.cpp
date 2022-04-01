/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rules.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 11:10:41 by pohl              #+#    #+#             */
/*   Updated: 2022/04/01 08:26:13 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/Rules.hpp"
#include "configParsing/helperFunctions.hpp"

Rules::Rules( void ) { return; }

Rules::Rules( Rules const & src ) { *this = src; return; }

Rules::~Rules( void ) { return; }

Rules &	Rules::operator=( Rules const & src )
{
	if (this == &src)
		return *this;
	this->locationPath = src.locationPath;
	this->root = src.root;
	this->allowedMethod = src.allowedMethod;
	this->autoindex = src.autoindex;
	this->cgiExtension = src.cgiExtension;
	this->cgiPath = src.cgiPath;
	this->clientMaxBodySize = src.clientMaxBodySize;
	this->errorPage = src.errorPage;
	this->index = src.index;
	this->listenPort = src.listenPort;
	this->listenAddress = src.listenAddress;
	this->redirectCode = src.redirectCode;
	this->redirectUri = src.redirectUri;
	this->uploadPath = src.uploadPath;
	this->serverName = src.serverName;
	return *this;
}

void	Rules::setValuesFromServerRules( const ServerRules& serverRules )
{
	this->autoindex = serverRules.autoindex;
	this->cgiExtension = serverRules.cgiExtension;
	this->cgiPath = serverRules.cgiPath;
	this->clientMaxBodySize = serverRules.clientMaxBodySize;
	this->errorPage = serverRules.errorPage;
	this->index = serverRules.index;
	this->listenPort = serverRules.listenPort;
	this->listenAddress = serverRules.listenAddress;
	this->serverName = serverRules.serverName;
}

void	Rules::setValuesFromLocationRules( const LocationRules* locationRules )
{
	if (locationRules)
	{
		this->allowedMethod = locationRules->allowedMethod;
		this->autoindex = locationRules->autoindex;
		this->cgiExtension = locationRules->cgiExtension;
		this->cgiPath = locationRules->cgiPath;
		this->clientMaxBodySize = locationRules->clientMaxBodySize;
		this->errorPage = locationRules->errorPage;
		this->index = locationRules->index;
		this->redirectCode = locationRules->redirectCode;
		this->redirectUri = locationRules->redirectUri;
		this->root = locationRules->root;
		this->uploadPath = locationRules->uploadPath;
		this->locationPath = locationRules->locationPath;
	}
	else
	{
		allowedMethod = GET | POST | DELETE;
		redirectCode = 0;
		redirectUri.clear();
		uploadPath.clear();
		locationPath.clear();
		root.clear();
	}
}

void	Rules::setValues( const ServerNode& server, std::string url )
{
	setValuesFromServerRules(server.getServerRules());
	setValuesFromLocationRules(server.getLocationFromUrl(url));
}

bool 	Rules::isMethodAllowed( char selectedMethod ) const
{
	if ((selectedMethod & allowedMethod) != 0)
		return true;
	return false;
}

bool	Rules::isMethodAllowed( std::string method ) const
{
	if (method == "GET")
		return isMethodAllowed(Rules::GET);
	else if (method == "POST")
		return isMethodAllowed(Rules::POST);
	else if (method == "DELETE")
		return isMethodAllowed(Rules::DELETE);
	else
		throw std::exception();
}

std::ostream &	operator<<( std::ostream & ostr, Rules const & instance)
{
	ostr << "The rules are:" << std::endl;
	ostr << "locationPath: " << instance.locationPath << std::endl;
	ostr << "root: " << instance.root << std::endl;
	ostr << "allowedMethod: " << instance.allowedMethod << std::endl;
	ostr << "autoindex: " << instance.autoindex << std::endl;
	ostr << "cgiExtension: " << instance.cgiExtension << std::endl;
	ostr << "cgiPath: " << instance.cgiPath << std::endl;
	ostr << "clientMaxBodySize: " << instance.clientMaxBodySize << std::endl;
	ostr << "errorPage:" << std::endl;
	for (std::map<int,std::string>::const_iterator it = instance.errorPage.begin();
			it != instance.errorPage.end(); it++)
		ostr << "	" << it->first << ": " << it->second << std::endl;
	ostr << "index:" << std::endl;
	for (std::vector<std::string>::const_iterator it = instance.index.begin();
			it != instance.index.end(); it++)
		ostr << "	- : " << *it << std::endl;
	ostr << "listenPort: " << instance.listenPort << std::endl;
	ostr << "listenAddress: " << instance.listenAddress << std::endl;
	ostr << "redirectCode: " << instance.redirectCode << std::endl;
	ostr << "redirectUri: " << instance.redirectUri << std::endl;
	ostr << "uploadPath: " << instance.uploadPath << std::endl;
	std::vector<std::string> 	serverName;
	return ostr;
}

bool		Rules::isCgi( std::string uri ) const
{
	return hasCgiExtension(uri, this->cgiExtension);
}
