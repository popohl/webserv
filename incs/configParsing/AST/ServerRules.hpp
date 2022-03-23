/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRules.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/22 15:34:18 by pohl             ###   ########.fr       */
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

	void		resetRules( void );

	static bool	verbose;

	void	addErrorPage( int errorCode, std::string errorPagePath );
	void	setIndex( std::vector<std::string> newIndex );
	void	addIndex( std::string newIndex );
	void	addServerName( std::string newServerName );

	bool						autoindex;
	int							clientMaxBodySize;
	std::string					cgiExtension;
	std::string					cgiPath;
	std::map<int, std::string>	errorPage;
	std::vector<std::string>	index;
	int							listenPort;
	std::string					listenAddress;
	std::vector<std::string> 	serverName;

};

std::ostream	&operator<<( std::ostream &ostr, const ServerRules &instance );

#endif
