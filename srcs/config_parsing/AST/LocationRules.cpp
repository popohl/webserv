/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/18 10:55:49 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "config_parsing/AST/LocationRules.hpp"

LocationRules::LocationRules( void )
{
	if (LocationRules::verbose)
		std::cout << "Default constructor for LocationRules called" << std::endl;
	reset_location_rules();
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
	reset_location_rules();
	this->autoindex = src.autoindex;
	this->client_max_body_size = src.client_max_body_size;
	this->error_page = src.error_page;
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
	this->allowed_method = src.allowed_method;
	this->autoindex = src.autoindex;
	this->cgi_extension = src.cgi_extension;
	this->cgi_path = src.cgi_path;
	this->client_max_body_size = src.client_max_body_size;
	this->error_page = src.error_page;
	this->index = src.index;
	this->redirect_code = src.redirect_code;
	this->redirect_uri = src.redirect_uri;
	this->root = src.root;
	this->upload_path = src.upload_path;
	return *this;
}

bool 	LocationRules::is_method_allowed( char selected_method ) const
{
	if ((selected_method & allowed_method) != 0)
		return true;
	return false;
}

bool	LocationRules::is_method_allowed( std::string method ) const
{
	if (method == "GET")
		return is_method_allowed(LocationRules::GET);
	else if (method == "POST")
		return is_method_allowed(LocationRules::POST);
	else if (method == "DELETE")
		return is_method_allowed(LocationRules::DELETE);
	else
		throw std::exception();
}

void	LocationRules::reset_location_rules( void )
{
	allowed_method = GET | POST | DELETE;
	autoindex = false;
	cgi_extension.clear();
	cgi_path.clear();
	client_max_body_size = 1048576;
	error_page.clear();
	index.clear();
	redirect_code = 0;
	redirect_uri.clear();
	root.clear();
	upload_path.clear();
}

bool	LocationRules::verbose = false;

void	LocationRules::allow_method( char method )
{
	this->allowed_method = this->allowed_method | method;
}

void	LocationRules::allow_method( std::string method )
{
	if (method == "GET")
		allow_method(LocationRules::GET);
	else if (method == "POST")
		allow_method(LocationRules::POST);
	else if (method == "DELETE")
		allow_method(LocationRules::DELETE);
	else
		throw std::exception();
}

void	LocationRules::forbid_method( char method )
{
	this->allowed_method = this->allowed_method & ~method;
}

void	LocationRules::forbid_method( std::string method )
{
	if (method == "GET")
		forbid_method(LocationRules::GET);
	else if (method == "POST")
		forbid_method(LocationRules::POST);
	else if (method == "DELETE")
		forbid_method(LocationRules::DELETE);
	else
		throw std::exception();
}

void	LocationRules::add_error_page( int error_code, std::string error_page_path )
{
	this->error_page[error_code] = error_page_path;
}
