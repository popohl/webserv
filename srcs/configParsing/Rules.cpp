/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rules.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 11:10:41 by pohl              #+#    #+#             */
/*   Updated: 2022/04/08 11:48:07 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responses/httpExceptions.hpp"
#include "configParsing/Rules.hpp"

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
		this->locationPath = locationRules->locationPath;
	}
	else
	{
		allowedMethod = GET | POST | DELETE;
		redirectCode = 0;
		redirectUri.clear();
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
	std::string errorMessage = "Method not implemented: ";
	errorMessage += method;
	throw serverError(501, errorMessage.c_str());
}

const std::string Rules::getServerName( void ) const
{
	if (serverName.empty())
		return "No server name";
	return serverName[0];
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
	return ostr;
}

bool		Rules::isCgi( const std::string& uri ) const
{
	char	afterExtension;
	size_t	extensionPosition;

	if (cgiExtension == "")
		return false;
	extensionPosition = uri.find("." + cgiExtension);
	if (extensionPosition == std::string::npos)
		return false;
	afterExtension = uri[extensionPosition + cgiExtension.size() + 1];
	if (afterExtension == 0 || afterExtension == '?' || afterExtension == '/')
		return true;
	return false;
}

std::string	Rules::getPathFromLocation( std::string pathFromUrl ) const
{
	if (root.empty())
		return pathFromUrl;

	std::string realPath = pathFromUrl;

	realPath.replace(0, locationPath.size(), root);
	return realPath;
}
