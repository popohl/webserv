/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFileNode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 14:52:06 by pohl              #+#    #+#             */
/*   Updated: 2022/03/18 12:26:43 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILENODE_HPP
# define CONFIGFILENODE_HPP

# include "configParsing/AST/ServerNode.hpp"
# include <vector>
# include <set>

class ConfigFileNode
{

public:

	ConfigFileNode( void );
	ConfigFileNode( const ConfigFileNode &src );
	~ConfigFileNode( void );

	ConfigFileNode	&operator=( const ConfigFileNode &src );

	std::vector<ServerNode>			&getServerList( void );
	const std::vector<ServerNode>	&getServerList( void ) const;
	std::set<int>					getListeningPorts( void ) const;
	ServerNode						&LatestServer( void );

	ServerNode&	createNewServerNode( void );

	static bool	verbose;

private:

	bool	alreadyContains( std::vector<int> listeningPorts, int port ) const;

	std::vector<ServerNode>	serverList;

};

#endif
