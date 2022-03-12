/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRule.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/10 16:44:12 by paulohl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONRULES_HPP
# define LOCATIONRULES_HPP

# include <iostream>
# include <vector>
# include <map>
# include "ServerRules.hpp"

class LocationRules
{

public:

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

private:

	char						_allowed_method;
	bool						_autoindex;
	std::string					_cgi_extension;
	std::string					_cgi_path;
	int							_client_max_body_size;
	std::map<int, std::string>	_error_page;
	std::vector<std::string>	_index;
	int							_redirect_code;
	std::string					_redirect_uri;
	std::string					_root;
	std::string					_upload_path;

};

std::ostream	&operator<<( std::ostream &ostr, const LocationRules &instance );

#endif
