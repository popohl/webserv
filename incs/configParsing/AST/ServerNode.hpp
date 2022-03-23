/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 14:54:43 by pohl              #+#    #+#             */
/*   Updated: 2022/03/22 15:43:59 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERNODE_HPP
# define SERVERNODE_HPP

# include "configParsing/AST/LocationRules.hpp"
# include "configParsing/AST/ServerRules.hpp"
# include <vector>

class ServerNode
{

public:

	ServerNode( void );
	ServerNode( const ServerNode &src );
	~ServerNode( void );

	ServerNode	&operator=( const ServerNode &src );

	LocationRules	&createNewLocationNode( void );

	std::vector<LocationRules>			&getLocationList( void );
	const std::vector<LocationRules>	&getLocationList( void ) const;
	ServerRules							&getServerRules( void );
	const ServerRules					&getServerRules( void ) const;
	LocationRules						&LatestLocation( void );

	const LocationRules*	getLocationFromUrl( std::string pathFromUrl ) const;
	bool					isCgi( std::string uri,
								LocationRules* location ) const;
	bool					hasCgiExtension( std::string uri,
								std::string cgiExtension ) const;

private:

	std::vector<LocationRules>	locationList;
	ServerRules					serverRules;

};

#endif
