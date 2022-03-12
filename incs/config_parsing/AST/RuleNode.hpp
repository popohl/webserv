/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleNode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 14:59:23 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 15:49:35 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RULENODE_HPP
# define RULENODE_HPP

# include "config_parsing/AST/AstNode.hpp"
# include <vector>

class RuleNode: public AstNode
{

public:

	RuleNode( void );
	RuleNode( const RuleNode &src );
	~RuleNode( void );

	RuleNode	&operator=( const RuleNode &src );

	static bool	verbose;

};

#endif
