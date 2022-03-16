/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRules.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/15 10:50:23 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parsing/AST/ServerRules.hpp"

ServerRules::ServerRules( void )
{
	if (ServerRules::verbose)
		std::cout << "Default constructor for ServerRules called" << std::endl;
	reset_rules();
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
	this->client_max_body_size = src.client_max_body_size;
	this->error_page = src.error_page;
	this->index = src.index;
	this->listen_port = src.listen_port;
	this->listen_address = src.listen_address;
	this->server_name = src.server_name;
	return *this;
}

void	ServerRules::reset_rules( void )
{
	autoindex = false;
	client_max_body_size = 1048576;
	error_page.clear();
	index.clear();
	listen_port = 8000;
	listen_address = "0.0.0.0";
	server_name.clear();
}

void	ServerRules::add_error_page( int error_code, std::string error_page_path )
{
	this->error_page[error_code] = error_page_path;
}

void	ServerRules::add_index( std::string new_index )
{
	this->index.push_back(new_index);
}

void	ServerRules::add_server_name( std::string new_server_name )
{
	this->server_name.push_back(new_server_name);
}

bool	ServerRules::verbose = false;
