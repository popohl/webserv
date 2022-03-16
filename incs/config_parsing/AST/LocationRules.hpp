/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/16 17:25:05 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONRULES_HPP
# define LOCATIONRULES_HPP

# include <iostream>
# include <vector>
# include <map>
# include "ServerRules.hpp"

struct LocationRules
{

	LocationRules( void );
	LocationRules( const LocationRules& src );
	LocationRules( const ServerRules& src );
	~LocationRules( void );

	LocationRules	&operator=( const LocationRules &rhs );

	void	reset_location_rules( void );

	bool 	is_method_allowed( char selected_method ) const;

	static bool	verbose;

	static const char	GET		= 0b0001;
	static const char	POST	= 0b0010;
	static const char	DELETE	= 0b0100;

	void	allow_method( std::string method );
	void	forbid_method( std::string method );
	void	add_error_page( int error_code, std::string error_page_path );

	std::string					locationPath;

	char						allowed_method;
	bool						autoindex;
	std::string					cgi_extension;
	std::string					cgi_path;
	int							client_max_body_size;
	std::map<int, std::string>	error_page;
	std::vector<std::string>	index;
	int							redirect_code;
	std::string					redirect_uri;
	std::string					root;
	std::string					upload_path;

};

std::ostream	&operator<<( std::ostream &ostr, const LocationRules &instance );

#endif
