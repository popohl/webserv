/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRule.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:34 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/10 16:55:19 by paulohl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "LocationRules.hpp"

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

	return *this;
}

bool 	LocationRules::is_method_allowed( char selected_method ) const
{
	if (selected_method & _allowed_method != 0)
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
