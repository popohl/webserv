/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/18 12:29:10 by pohl             ###   ########.fr       */
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

	void	resetLocationRules( void );

	bool 	isMethodAllowed( char selectedMethod ) const;
	bool 	isMethodAllowed( std::string selectedMethod ) const;

	static bool	verbose;

	static const char	GET			= 0b0001;
	static const char	POST		= 0b0010;
	static const char	DELETE		= 0b0100;
	static const char	ALL_METHODS	= 0b0111;

	void	allowMethod( std::string method );
	void	allowMethod( char method );
	void	forbidMethod( std::string method );
	void	forbidMethod( char method );
	void	addErrorPage( int errorCode, std::string errorPagePath );

	std::string					locationPath;

	char						allowedMethod;
	bool						autoindex;
	std::string					cgiExtension;
	std::string					cgiPath;
	int							clientMaxBodySize;
	std::map<int, std::string>	errorPage;
	std::vector<std::string>	index;
	int							redirectCode;
	std::string					redirectUri;
	std::string					root;
	std::string					uploadPath;

};

#endif
