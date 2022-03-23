/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 15:32:42 by pohl              #+#    #+#             */
/*   Updated: 2022/03/22 15:28:08 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/AST/ServerNode.hpp"

ServerNode::ServerNode( void )
{
	locationList.clear();
	return;
}

ServerNode::ServerNode( ServerNode const & src )
{
	*this = src;
	return;
}

ServerNode::~ServerNode( void )
{
	return;
}

ServerNode &	ServerNode::operator=( ServerNode const & src )
{
	if (this == &src)
		return *this;
	this->locationList = src.locationList;
	this->serverRules = src.serverRules;
	return *this;
}

LocationRules	&ServerNode::createNewLocationNode( void )
{
	LocationRules	newLocation(serverRules);

	locationList.push_back(newLocation);
	return locationList.back();
}

LocationRules	&ServerNode::LatestLocation( void )
{
	return locationList.back();
}

std::vector<LocationRules>			&ServerNode::getLocationList( void )
{
	return locationList;
}

const std::vector<LocationRules>	&ServerNode::getLocationList( void ) const
{
	return locationList;
}

ServerRules							&ServerNode::getServerRules( void )
{
	return serverRules;
}

const ServerRules					&ServerNode::getServerRules( void ) const
{
	return serverRules;
}

const LocationRules*
	ServerNode::getLocationFromUrl( std::string pathFromUrl ) const
{
	const LocationRules* result = NULL;

	for (std::vector<LocationRules>::const_iterator it = locationList.begin();
			it != locationList.end(); it++)
	{
		if (pathFromUrl.rfind(it->locationPath, 0) == 0)
		{
			if (result == NULL
					|| it->locationPath.size() > result->locationPath.size())
				result = &(*it);
		}
	}
	return result;
}

bool		ServerNode::hasCgiExtension( std::string uri,
		std::string cgiExtension ) const
{
	if (cgiExtension == "")
		return false;
	uri.erase(0, uri.find_first_of('.') + 1);
	if (uri.rfind(cgiExtension, 0) == 0)
		return true;
	return false;
}

bool		ServerNode::isCgi( std::string uri, LocationRules* location ) const
{
	if (location == NULL)
		return hasCgiExtension(uri, this->getServerRules().cgiExtension);
	return location->isCgi(uri);
}
