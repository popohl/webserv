/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:00:56 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 13:31:00 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/Lexer.hpp"

void	test_file_opening( std::string filename )
{
	try {
		Lexer	standard_constructor(filename);
	}
	catch ( Lexer::file_exception &e ) {
		std::cout << "Good error: " << e.what() << std::endl;
	}
	catch ( std::exception &e ) {
		std::cout << "Wrong error: " << e.what() << std::endl;
	}
}

void	test_file_reading( std::string filename )
{
	Token	token;

	try {
		Lexer	lexer(filename);

		do
		{
			try {
				token = lexer.get_next_token();
			}
			catch ( Lexer::token_exception &e ) {
				std::cout << "Good error: " << e.what() << std::endl;
				lexer.advance();
			}
		} while (token.getType() != Token::end_of_file);
	}
	catch ( Lexer::file_exception &e ) {
		std::cout << "Good error: " << e.what() << std::endl;
	}
}

void	lexer_tests()
{
	Lexer		empty_constructor;

	/* test_file_opening("Bonjour"); */
	/* test_file_opening("Makefile"); */
	test_file_reading("./config_files/single_elements.conf");
	/* test_file_opening("./config_files/config.conf"); */
}
