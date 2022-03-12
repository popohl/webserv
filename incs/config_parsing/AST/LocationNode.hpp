/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationNode.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 14:58:03 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 15:01:49 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONNODE_HPP
# define LOCATIONNODE_HPP

# include "config_parsing/AST/AstNode.hpp"
# include "config_parsing/AST/RuleNode.hpp"
# include <vector>

class LocationNode: public AstNode
{

public:

	LocationNode( void );
	LocationNode( const LocationNode &src );
	~LocationNode( void );

	LocationNode	&operator=( const LocationNode &src );

	static bool	verbose;

private:

	std::vector<RuleNode>		serverRules;

};

#endif
