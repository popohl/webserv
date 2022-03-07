/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 10:00:33 by pohl              #+#    #+#             */
/*   Updated: 2022/03/07 14:19:39 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/Lexer.hpp"

Lexer::Lexer( void )
{
	if (Lexer::verbose)
		std::cout << "Empty constructor for Lexer called" << std::endl;
	return;
}

Lexer::Lexer( std::string& file_name )
{
	if (Lexer::verbose)
		std::cout << "Standard constructor for Lexer called" << std::endl;
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
	if (Lexer::verbose)
		std::cout << "Opening file: " << file_name << std::endl;
	this->_config_file.open(file_name.c_str());
	if (this->_config_file.bad())
		throw Lexer::file_exception();
}

bool	Lexer::verbose = true;
