/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_tests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:04:35 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 14:34:55 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parsing/Token.hpp"

void	token_tests( void )
{
	Token	empty_constructor;
	Token	standard_constructor(Token::word, "bonjour");
	Token	copy_constructor(standard_constructor);

	if (standard_constructor != copy_constructor)
		throw std::logic_error("copy_constructor");
	empty_constructor = copy_constructor;
	if (empty_constructor != copy_constructor)
		throw std::logic_error("assignation overload");
	std::cout << standard_constructor << std::endl;
}
