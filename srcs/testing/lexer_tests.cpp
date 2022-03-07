/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:00:56 by pohl              #+#    #+#             */
/*   Updated: 2022/03/07 12:34:02 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/Lexer.hpp"

void	lexer_tests()
{
	Lexer	empty_constructor;

	std::string filename = "hello";
	Lexer	standard_constructor(filename);
}
