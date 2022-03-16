/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/16 16:08:22 by fmonbeig         ###   ########.fr       */
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
	this->_autoindex = src.get_autoindex();
	this->_client_max_body_size = src.get_client_max_body_size();
	this->_error_page = src.get_error_page();
	this->_index = src.get_index();
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

	this->_allowed_method = src._allowed_method;
	this->_autoindex = src._autoindex;
	this->_cgi_extension = src._cgi_extension;
	this->_cgi_path = src._cgi_path;
	this->_client_max_body_size = src._client_max_body_size;
	this->_error_page = src._error_page;
	this->_index = src._index;
	this->_redirect_code = src._redirect_code;
	this->_redirect_uri = src._redirect_uri;
	this->_root = src._root;
	this->_upload_path = src._upload_path;

	return *this;
}

bool 	LocationRules::is_method_allowed( char selected_method ) const
{
	if ((selected_method & _allowed_method) != 0)
		return true;
	return false;
}

void	LocationRules::reset_location_rules( void )
{
	_allowed_method = GET | POST | DELETE;
	_autoindex = false;
	_cgi_extension.clear();
	_cgi_path.clear();
	_client_max_body_size = 1048576;
	_error_page.clear();
	_index.clear();
	_redirect_code = 0;
	_redirect_uri.clear();
	_root.clear();
	_upload_path.clear();
}

bool	LocationRules::verbose = false;

char	LocationRules::get_allowed_method( void ) const
{
	return this->_allowed_method;
}

bool	LocationRules::get_autoindex( void ) const
{
	return this->_autoindex;
}

std::string	LocationRules::get_cgi_extension( void ) const
{
	return this->_cgi_extension;
}

std::string	LocationRules::get_cgi_path( void ) const
{
	return this->_cgi_path;
}

int	LocationRules::get_client_max_body_size( void ) const
{
	return this->_client_max_body_size;
}

std::map<int,std::string>	LocationRules::get_error_page( void ) const
{
	return this->_error_page;
}

std::vector<std::string>	LocationRules::get_index( void ) const
{
	return this->_index;
}

int	LocationRules::get_redirect_code( void ) const
{
	return this->_redirect_code;
}

std::string	LocationRules::get_redirect_uri( void ) const
{
	return this->_redirect_uri;
}

std::string	LocationRules::get_root( void ) const
{
	return this->_root;
}

std::string	LocationRules::get_upload_path( void ) const
{
	return this->_upload_path;
}

void	LocationRules::set_allowed_method( char new_allowed_method )
{
	this->_allowed_method = new_allowed_method;
}

void	LocationRules::allow_method( std::string method )
{
	if (method == "GET")
		this->_allowed_method = this->_allowed_method | LocationRules::GET;
	else if (method == "POST")
		this->_allowed_method = this->_allowed_method | LocationRules::POST;
	else if (method == "DELETE")
		this->_allowed_method = this->_allowed_method | LocationRules::DELETE;
	else
		throw std::exception();
}

void	LocationRules::forbid_method( std::string method )
{
	if (method == "GET")
		this->_allowed_method = this->_allowed_method & ~LocationRules::GET;
	else if (method == "POST")
		this->_allowed_method = this->_allowed_method & ~LocationRules::POST;
	else if (method == "DELETE")
		this->_allowed_method = this->_allowed_method & ~LocationRules::DELETE;
	else
		throw std::exception();
}

void	LocationRules::set_autoindex( bool new_autoindex )
{
	this->_autoindex = new_autoindex;
}

void	LocationRules::set_cgi_extension( std::string new_cgi_extension )
{
	this->_cgi_extension = new_cgi_extension;
}

void	LocationRules::set_cgi_path( std::string new_cgi_path )
{
	this->_cgi_path = new_cgi_path;
}

void	LocationRules::set_client_max_body_size( int new_client_max_body_size )
{
	if (new_client_max_body_size < 0)
		throw std::exception();
	this->_client_max_body_size = new_client_max_body_size;
}

void	LocationRules::set_error_page( std::map<int, std::string> new_error_page )
{
	this->_error_page = new_error_page;
}

void	LocationRules::add_error_page( int error_code, std::string error_page_path )
{
	if (!ServerRules::is_error_code_valid(error_code))
		throw std::exception();
	this->_error_page[error_code] = error_page_path;
}

void	LocationRules::set_index( std::vector<std::string> new_index )
{
	this->_index = new_index;
}

void	LocationRules::set_redirect_code( int new_redirect_code )
{
	if (new_redirect_code < 300 || new_redirect_code > 308)
		throw std::exception();
	this->_redirect_code = new_redirect_code;
}

void	LocationRules::set_redirect_uri( std::string new_redirect_uri )
{
	this->_redirect_uri = new_redirect_uri;
}

void	LocationRules::set_root( std::string new_root )
{
	this->_root = new_root;
}

void	LocationRules::set_upload_path( std::string new_upload_path )
{
	this->_upload_path = new_upload_path;
}
