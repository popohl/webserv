/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 14:54:43 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 16:35:50 by pohl             ###   ########.fr       */
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
	LocationRules						&latestLocation( void );

	const LocationRules*	getLocationFromUrl( std::string pathFromUrl ) const;
	bool	isCgi( std::string uri, const LocationRules* location ) const;

private:

	std::vector<LocationRules>	locationList;
	ServerRules					serverRules;

};

#endif
