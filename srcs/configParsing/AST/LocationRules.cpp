/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/04/05 10:21:23 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/AST/LocationRules.hpp"

LocationRules::LocationRules( void )
{
	resetRules();
	return;
}

LocationRules::LocationRules( LocationRules const & src )
{
	*this = src;
	return;
}

LocationRules::LocationRules( const ServerRules& src )
{
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
	return;
}

LocationRules &	LocationRules::operator=( LocationRules const & src )
{
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
