/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRules.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/16 19:55:15 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_RULES_HPP
# define SERVER_RULES_HPP

# include <iostream>
# include <vector>
# include <map>

struct ServerRules
{

	ServerRules( void );
	ServerRules( const ServerRules &src );
	~ServerRules( void );

	ServerRules	&operator=( const ServerRules &rhs );

	void		reset_rules( void );

	static bool	verbose;

	void	add_error_page( int error_code, std::string error_page_path );
	void	set_index( std::vector<std::string> new_index );
	void	add_index( std::string new_index );
	void	add_server_name( std::string new_server_name );

	bool						autoindex;
	int							client_max_body_size;
	std::map<int, std::string>	error_page;
	std::vector<std::string>	index;
	int							listen_port;
	std::string					listen_address;
	std::vector<std::string> 	server_name;

};

std::ostream	&operator<<( std::ostream &ostr, const ServerRules &instance );

#endif
