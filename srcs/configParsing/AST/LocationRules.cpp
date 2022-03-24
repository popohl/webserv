/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/23 12:09:36 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/AST/LocationRules.hpp"
#include "configParsing/helperFunctions.hpp"

LocationRules::LocationRules( void )
{
	if (LocationRules::verbose)
		std::cout << "Default constructor for LocationRules called" << std::endl;
	resetRules();
	return;
}

LocationRules::LocationRules( LocationRules const & src )
{
	if (LocationRules::verbose)
		std::cout << "Copy constructor for LocationRules called" << std::endl;
	*this = src;
	return;
}

LocationRules::LocationRules( const ServerRules& src )
{
	if (LocationRules::verbose)
		std::cout << "ServerRules copy constructor for LocationRules called" << std::endl;
	resetRules();
	this->autoindex = src.autoindex;
	this->clientMaxBodySize = src.clientMaxBodySize;
	this->cgiExtension = src.cgiExtension;
	this->cgiPath = src.cgiPath;
	this->errorPage = src.errorPage;
	this->index = src.index;
	return;
}

LocationRules::~LocationRules( void )
{
	if (LocationRules::verbose)
		std::cout << "Destructor for LocationRules called" << std::endl;
	return;
}

LocationRules &	LocationRules::operator=( LocationRules const & src )
{
	if (LocationRules::verbose)
		std::cout << "Assignement operator for LocationRules called" << std::endl;
	if (this == &src)
		return *this;
	this->allowedMethod = src.allowedMethod;
	this->autoindex = src.autoindex;
	this->cgiExtension = src.cgiExtension;
	this->cgiPath = src.cgiPath;
	this->clientMaxBodySize = src.clientMaxBodySize;
	this->errorPage = src.errorPage;
	this->index = src.index;
	this->redirectCode = src.redirectCode;
	this->redirectUri = src.redirectUri;
	this->root = src.root;
	this->uploadPath = src.uploadPath;
	this->locationPath = src.locationPath;
	return *this;
}

bool 	LocationRules::isMethodAllowed( char selectedMethod ) const
{
	if ((selectedMethod & allowedMethod) != 0)
		return true;
	return false;
}

bool	LocationRules::isMethodAllowed( std::string method ) const
{
	if (method == "GET")
		return isMethodAllowed(LocationRules::GET);
	else if (method == "POST")
		return isMethodAllowed(LocationRules::POST);
	else if (method == "DELETE")
		return isMethodAllowed(LocationRules::DELETE);
	else
		throw std::exception();
}

void	LocationRules::resetRules( void )
{
	allowedMethod = GET | POST | DELETE;
	autoindex = false;
	cgiExtension.clear();
	cgiPath.clear();
	clientMaxBodySize = 1048576;
	errorPage.clear();
	index.clear();
	locationPath.clear();
	redirectCode = 0;
	redirectUri.clear();
	root.clear();
	uploadPath.clear();
}

bool	LocationRules::verbose = false;

void	LocationRules::allowMethod( char method )
{
	this->allowedMethod = this->allowedMethod | method;
}

void	LocationRules::allowMethod( std::string method )
{
	if (method == "GET")
		allowMethod(LocationRules::GET);
	else if (method == "POST")
		allowMethod(LocationRules::POST);
	else if (method == "DELETE")
		allowMethod(LocationRules::DELETE);
	else
		throw std::exception();
}

void	LocationRules::forbidMethod( char method )
{
	this->allowedMethod = this->allowedMethod & ~method;
}

void	LocationRules::forbidMethod( std::string method )
{
	if (method == "GET")
		forbidMethod(LocationRules::GET);
	else if (method == "POST")
		forbidMethod(LocationRules::POST);
	else if (method == "DELETE")
		forbidMethod(LocationRules::DELETE);
	else
		throw std::exception();
}

void	LocationRules::addErrorPage( int errorCode, std::string errorPagePath )
{
	this->errorPage[errorCode] = errorPagePath;
}

std::string	LocationRules::getPathFromLocation( std::string pathFromUrl ) const
{
	if (root.empty())
		return pathFromUrl;

	std::string result = pathFromUrl;

	result.replace(0, locationPath.size(), root);
	return result;
}

bool		LocationRules::isCgi( std::string uri ) const
{
	return hasCgiExtension(uri, this->cgiExtension);
}
