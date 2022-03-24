/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRulesNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:34:22 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 16:08:23 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRULENODE_HPP
# define IRULENODE_HPP

# include <iostream>
# include <vector>
# include <map>

struct IRulesNode
{

	IRulesNode( void ) {}
	virtual ~IRulesNode( void ) {}

	virtual void		resetRules( void ) = 0;

	virtual void	addErrorPage( int errCode, std::string errPagePath ) = 0;
	virtual void	addIndex( std::string newIndex ) = 0;

	bool						autoindex;
	int							clientMaxBodySize;
	std::string					cgiExtension;
	std::string					cgiPath;
	std::map<int, std::string>	errorPage;
	std::vector<std::string>	index;

};

#endif
