/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileNode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 14:52:06 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 14:13:15 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILENODE_HPP
# define CONFIGFILENODE_HPP

# include "configParsing/AST/ServerNode.hpp"
# include <vector>
# include <set>
# include <map>

typedef std::map<int, std::vector<ServerNode *> >	mapPortToServers;

class ConfigFileNode
{

public:

	ConfigFileNode( void );
	ConfigFileNode( const ConfigFileNode &src );
	~ConfigFileNode( void );

	ConfigFileNode	&operator=( const ConfigFileNode &src );

	std::vector<ServerNode>			&getServerList( void );
	const std::vector<ServerNode>	&getServerList( void ) const;
	mapPortToServers				getListeningPorts( void );
	ServerNode						&latestServer( void );

	ServerNode&	createNewServerNode( void );

private:

	bool	alreadyContains( std::vector<int> listeningPorts, int port ) const;

	std::vector<ServerNode>	serverList;

};

#endif
