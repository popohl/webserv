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

#ifndef LOCATIONRULE_HPP
# define LOCATIONRULE_HPP

# include <iostream>
# include <vector>
# include <map>

class LocationRule
{

public:

	LocationRule( void );
	LocationRule( const LocationRule &src );
	~LocationRule( void );

	LocationRule	&operator=( const LocationRule &rhs );

	void	reset_location_rules( void );

	static bool	verbose;

	static const char	GET		= 0b0001;
	static const char	POST	= 0b0010;
	static const char	DELETE	= 0b0100;

private:

	char						_allowed_method;
	int							_client_max_body_size;
	std::map<int, std::string>	_error_page;
	std::string					_root;
	std::vector<std::string>	_index;
	std::string					_upload_path;
	bool						_autoindex;
	std::string					_cgi_extension;
	std::string					_cgi_path;
	std::pair<int, std::string>	_redirect;

};

std::ostream	&operator<<( std::ostream &ostr, const LocationRule &instance );

#endif
