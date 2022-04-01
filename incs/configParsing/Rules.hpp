/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rules.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 11:09:00 by pohl              #+#    #+#             */
/*   Updated: 2022/04/01 08:25:06 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RULES_HPP
# define RULES_HPP

# include <iostream>
# include <map>
# include <vector>
# include "configParsing/AST/ServerNode.hpp"

struct Rules
{

	Rules( void );
	~Rules( void );

	void	setValues( const ServerNode& server, std::string url );

	static const char	GET			= 0b0001;
	static const char	POST		= 0b0010;
	static const char	DELETE		= 0b0100;
	static const char	ALL_METHODS	= 0b0111;

	bool 	isMethodAllowed( char selectedMethod ) const;
	bool 	isMethodAllowed( std::string selectedMethod ) const;

	bool	isCgi( std::string uri ) const;

	std::string					locationPath;
	std::string					root;
	char						allowedMethod;
	bool						autoindex;
	std::string					cgiExtension;
	std::string					cgiPath;
	int							clientMaxBodySize;
	std::map<int, std::string>	errorPage;
	std::vector<std::string>	index;
	int							listenPort;
	std::string					listenAddress;
	int							redirectCode;
	std::string					redirectUri;
	std::string					uploadPath;
	std::vector<std::string> 	serverName;

private:

	Rules( const Rules &src );
	Rules	&operator=( const Rules &src );

	void	setValuesFromServerRules( const ServerRules& serverRules );
	void	setValuesFromLocationRules( const LocationRules* locationRules );

};

std::ostream	&operator<<( std::ostream &ostr, const Rules &instance );

#endif
