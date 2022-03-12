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


#include "LocationRule.hpp"

LocationRule::LocationRule( void )
{
	if (LocationRule::verbose)
		std::cout << "Default constructor for LocationRule called" << std::endl;
	reset_location_rules();
	return;
}

LocationRule::LocationRule( LocationRule const & src )
{
	if (LocationRule::verbose)
		std::cout << "Copy constructor for LocationRule called" << std::endl;
	*this = src;
	return;
}

LocationRule::~LocationRule( void )
{
	if (LocationRule::verbose)
		std::cout << "Destructor for LocationRule called" << std::endl;
	return;
}

LocationRule &	LocationRule::operator=( LocationRule const & src )
{
	if (LocationRule::verbose)
		std::cout << "Assignement operator for LocationRule called" << std::endl;
	if (this == &src)
		return *this;

	return *this;
}

void	LocationRule::reset_location_rules( void )
{
	_allowed_method = GET | POST | DELETE;
	_client_max_body_size = 1048576;
	_error_page.clear();
	_root.clear();
	_index.clear();
	_upload_path.clear();
	_autoindex = false;
	_cgi_extension.clear();
	_cgi_path.clear();
	_redirect.first = 0;
	_redirect.second.clear();
}

bool	LocationRule::verbose = false;
