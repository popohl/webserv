/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/12 18:59:08 by pohl             ###   ########.fr       */
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

	char						get_allowed_method( void ) const;
	bool						get_autoindex( void ) const;
	std::string					get_cgi_extension( void ) const;
	std::string					get_cgi_path( void ) const;
	int							get_client_max_body_size( void ) const;
	std::map<int, std::string>	get_error_page( void ) const;
	std::vector<std::string>	get_index( void ) const;
	int							get_redirect_code( void ) const;
	std::string					get_redirect_uri( void ) const;
	std::string					get_root( void ) const;
	std::string					get_upload_path( void ) const;

	void	allow_method( std::string method );
	void	forbid_method( std::string method );
	void	set_autoindex( bool new_autoindex );
	void	set_cgi_extension( std::string new_cgi_extension );
	void	set_cgi_path( std::string new_cgi_path );
	void	set_client_max_body_size( int new_client_max_body_size );
	void	add_error_page( int error_code, std::string error_page_path );
	void	set_index( std::vector<std::string> new_index );
	void	set_redirect_code( int new_redirect_code );
	void	set_redirect_uri( std::string new_redirect_uri );
	void	set_root( std::string new_root );
	void	set_upload_path( std::string new_upload_path );

private:

	void	set_error_page( std::map<int, std::string> );
	void	set_allowed_method( char new_allowed_method );

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
