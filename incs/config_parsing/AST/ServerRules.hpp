/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRules.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/12 19:07:26 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_RULES_HPP
# define SERVER_RULES_HPP

# include <iostream>
# include <vector>
# include <map>

class ServerRules
{

public:

	ServerRules( void );
	ServerRules( const ServerRules &src );
	~ServerRules( void );

	ServerRules	&operator=( const ServerRules &rhs );

	static bool	is_error_code_valid( int error_code );

	void	reset_location_rules( void );

	static bool	verbose;

	bool						get_autoindex( void ) const;
	int							get_client_max_body_size( void ) const;
	std::map<int, std::string>	get_error_page( void ) const;
	std::vector<std::string>	get_index( void ) const;
	int							get_listen_port( void ) const;
	std::string					get_listen_address( void ) const;
	std::vector<std::string> 	get_server_name( void ) const;

	void	set_autoindex( bool new_autoindex );
	void	set_client_max_body_size( int new_client_max_body_size );

	void	set_error_page( std::map<int, std::string> new_error_page );
	void	add_error_page( int error_code, std::string error_page_path );
	void	set_index( std::vector<std::string> new_index );
	void	add_index( std::string new_index );
	void	set_listen_port( int new_listen_port );
	void	set_listen_address( std::string new_listen_address );
	void	set_server_name( std::vector<std::string> new_server_name );
	void	add_server_name( std::string new_server_name );

private:

	void	set_server_name( std::vector<std::string> new_server_name );
	void	set_index( std::vector<std::string> new_index );
	void	set_error_page( std::map<int, std::string> new_error_page );

	bool						_autoindex;
	int							_client_max_body_size;
	std::map<int, std::string>	_error_page;
	std::vector<std::string>	_index;
	int							_listen_port;
	std::string					_listen_address;
	std::vector<std::string> 	_server_name;

};

std::ostream	&operator<<( std::ostream &ostr, const ServerRules &instance );

#endif
