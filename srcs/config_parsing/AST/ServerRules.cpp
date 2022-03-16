/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRules.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/16 16:03:39 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parsing/AST/ServerRules.hpp"

ServerRules::ServerRules( void )
{
	if (ServerRules::verbose)
		std::cout << "Default constructor for ServerRules called" << std::endl;
	reset_location_rules();
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

	return *this;
}

bool	ServerRules::is_error_code_valid( int error_code )
{
	(void)error_code;
	return true;
}

void	ServerRules::reset_location_rules( void )
{
	_autoindex = false;
	_client_max_body_size = 1048576;
	_error_page.clear();
	_index.clear();
	_listen_port = 8000;
	_listen_address = "0.0.0.0";
	_server_name.clear();
}

bool						ServerRules::get_autoindex( void ) const
{
	return this->_autoindex;
}

int							ServerRules::get_client_max_body_size( void ) const
{
	return this->_client_max_body_size;
}

std::map<int, std::string>	ServerRules::get_error_page( void ) const
{
	return this->_error_page;
}

std::vector<std::string>	ServerRules::get_index( void ) const
{
	return this->_index;
}

int							ServerRules::get_listen_port( void ) const
{
	return this->_listen_port;
}

std::string					ServerRules::get_listen_address( void ) const
{
	return this->_listen_address;
}

std::vector<std::string> 	ServerRules::get_server_name( void ) const
{
	return this->_server_name;
}

void	ServerRules::set_autoindex( bool new_autoindex )
{
	this->_autoindex = new_autoindex;
}

void	ServerRules::set_client_max_body_size( int new_client_max_body_size )
{
	this->_client_max_body_size = new_client_max_body_size;
}

void	ServerRules::set_error_page( std::map<int, std::string> new_error_page )
{
	this->_error_page = new_error_page;
}

void	ServerRules::add_error_page( int error_code, std::string error_page_path )
{
	if (!is_error_code_valid(error_code))
		throw std::exception();
	this->_error_page[error_code] = error_page_path;
}

void	ServerRules::set_index( std::vector<std::string> new_index )
{
	this->_index = new_index;
}

void	ServerRules::add_index( std::string new_index )
{
	this->_index.push_back(new_index);
}

void	ServerRules::set_listen_port( int new_listen_port )
{
	if (new_listen_port <= 0)
		throw std::exception();
	this->_listen_port = new_listen_port;
}

void	ServerRules::set_listen_address( std::string new_listen_address )
{
	this->_listen_address = new_listen_address;
}

void	ServerRules::set_server_name( std::vector<std::string> new_server_name )
{
	this->_server_name = new_server_name;
}

void	ServerRules::add_server_name( std::string new_server_name )
{
	this->_server_name.push_back(new_server_name);
}

bool	ServerRules::verbose = false;
