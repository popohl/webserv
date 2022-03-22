/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRules.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paulohl <pohl@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:11:02 by paulohl           #+#    #+#             */
/*   Updated: 2022/03/22 15:32:25 by pohl             ###   ########.fr       */
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

	void	resetRules( void );

	static bool	verbose;

	static const char	GET			= 0b0001;
	static const char	POST		= 0b0010;
	static const char	DELETE		= 0b0100;
	static const char	ALL_METHODS	= 0b0111;

	void	allowMethod( std::string method );
	void	allowMethod( char method );
	void	forbidMethod( std::string method );
	void	forbidMethod( char method );
	bool 	isMethodAllowed( char selectedMethod ) const;
	bool 	isMethodAllowed( std::string selectedMethod ) const;

	void	addErrorPage( int errorCode, std::string errorPagePath );

	// Helper functions
	std::string	getPathFromLocation( std::string pathFromUrl ) const;
	bool		isCgi( std::string uri ) const;

	std::string					locationPath;
	std::string					root;

	char						allowedMethod;
	bool						autoindex;
	std::string					cgiExtension;
	std::string					cgiPath;
	int							clientMaxBodySize;
	std::map<int, std::string>	errorPage;
	std::vector<std::string>	index;
	int							redirectCode;
	std::string					redirectUri;
	std::string					uploadPath;

};

#endif
