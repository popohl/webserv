/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 10:00:33 by pohl              #+#    #+#             */
/*   Updated: 2022/03/07 12:33:00 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/Lexer.hpp"

Lexer::Lexer( void )
{
	if (Lexer::verbose)
		std::cout << "Default constructor for Lexer called" << std::endl;
	return;
}

Lexer::Lexer( std::string& file_name )
{
	this->open_file(file_name);
}

Lexer::~Lexer( void )
{
	if (Lexer::verbose)
		std::cout << "Destructor for Lexer called" << std::endl;
	return;
}

void	Lexer::open_file( std::string& file_name )
{
	this->_config_file.open(file_name.c_str());
	if (this->_config_file.fail())
		throw Lexer::file_exception();
}

bool	Lexer::verbose = true;
