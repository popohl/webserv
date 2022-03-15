/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 14:54:43 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 14:57:54 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERNODE_HPP
# define SERVERNODE_HPP

# include "config_parsing/AST/LocationRules.hpp"
# include "config_parsing/AST/ServerRules.hpp"
# include <vector>

class ServerNode
{

public:

	ServerNode( void );
	ServerNode( const ServerNode &src );
	~ServerNode( void );

	ServerNode	&operator=( const ServerNode &src );

	static bool	verbose;

private:

	std::vector<LocationRules>	locationList;
	std::vector<ServerRules>	serverRules;

};

#endif
