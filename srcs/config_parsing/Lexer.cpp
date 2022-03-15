/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 10:00:33 by pohl              #+#    #+#             */
/*   Updated: 2022/03/14 16:04:13 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parsing/Lexer.hpp"
#include <limits>
#include <cctype>

Lexer::Lexer( void )
{
	if (Lexer::verbose)
		std::cout << "Empty constructor for Lexer called" << std::endl;
	return;
}

Lexer::Lexer( std::string file_name )
{
	if (Lexer::verbose)
		std::cout << "Standard constructor for Lexer called" << std::endl;
	this->open_file(file_name);
}

Lexer::~Lexer( void )
{
	if (Lexer::verbose)
		std::cout << "Destructor for Lexer called" << std::endl;
	this->close_file();
	return;
}

Token	Lexer::get_next_token( void )
{
	skip_whitespaces_and_comments();
	if (reached_eof())
		return tokenize_single_character(Token::end_of_file);
	if (_current_char == '{')
		return tokenize_single_character(Token::opening_bracket);
	if (_current_char == '}')
		return tokenize_single_character(Token::closing_bracket);
	if (_current_char == ';')
		return tokenize_single_character(Token::semicolon);
	if (is_path_special_char(_current_char))
		return tokenize_path();
	if (isalpha(_current_char))
		return tokenize_word();
	if (isdigit(_current_char))
		return differentiate_digit_tokens();
	throw Lexer::token_exception();
}

Token	Lexer::tokenize_single_character( Token::token_type type )
{
	advance();
	return Token(type, "");
}

Token	Lexer::tokenize_word( void )
{
	std::string	word;

	do
	{
		word += advance();
	} while (isalpha(_current_char) || _current_char == '.' || _current_char == '_');
	return Token(Token::word, word);
}

Token	Lexer::differentiate_digit_tokens( void )
{
	std::string	number;

	number += get_number();
	if (_current_char == '.')
		return differentiate_number_size(number);
	else if (is_size_prefix(_current_char) || _current_char == 'b'
			|| _current_char == 'B')
		return tokenize_size(number);
	return Token(Token::number, number);
}

Token	Lexer::differentiate_number_size( std::string &beginning )
{
	std::string	ip(beginning);

	if (_current_char == '.')
		ip += advance();
	else
		throw Lexer::token_exception();
	ip += get_number();
	if (_current_char == '.')
		ip += advance();
	else
		return tokenize_size(ip);
	ip += get_number();
	if (_current_char == '.')
		ip += advance();
	else
		throw Lexer::token_exception();
	ip += get_number();
	return Token(Token::ip_address, ip);
}

Token	Lexer::tokenize_size( std::string &number_part )
{
	std::string	size(number_part);

	if (is_size_prefix(_current_char))
		size += get_size_multiplier();
	else if (_current_char == 'b' || _current_char == 'B')
	{
		advance();
		return Token(Token::size, size);
	}
	else
		throw Lexer::token_exception();
	if (_current_char == 'i')
		advance();
	if (_current_char == 'b' || _current_char == 'B')
		advance();
	return Token(Token::size, size);
}

char	Lexer::get_size_multiplier( void )
{
	char		tmp_uppercase;

	tmp_uppercase = advance();
	if (tmp_uppercase == 'k' || tmp_uppercase == 'm' || tmp_uppercase == 'g'
			|| tmp_uppercase == 't')
		tmp_uppercase -= 32;
	return tmp_uppercase;
}

Token	Lexer::tokenize_path( void )
{
	std::string	path;

	do
	{
		path += advance();
	} while (isalnum(_current_char) || is_path_special_char(_current_char));
	return Token(Token::path, path);
}

std::string	Lexer::get_number( void )
{
	std::string	number;

	while (isdigit(_current_char))
		number += advance();
	return number;
}

void	Lexer::open_file( std::string file_name )
{
	this->close_file();
	if (Lexer::verbose)
		std::cout << "Opening file: " << file_name << std::endl;
	this->_config_file.open(file_name.c_str());
	if (this->_config_file.fail())
		throw Lexer::file_exception();
	else if (Lexer::verbose)
		std::cout << "Success opening file" << std::endl;
	read_first_character();
}

void	Lexer::close_file( void )
{
	if (Lexer::verbose)
		std::cout << "closing current file" << std::endl;
	if (this->_config_file.is_open())
		this->_config_file.close();
}

char	Lexer::advance( void )
{
	char tmp = _current_char;
	_current_char = _config_file.get();
	return tmp;
}

void	Lexer::read_first_character( void )
{
	advance();
}

bool	Lexer::reached_eof( void ) const
{
	return _config_file.eof();
}

bool	Lexer::is_size_prefix( char c ) const
{
	return c == 'm' || c == 'M' || c == 'k' || c == 'K' || c == 'g' || c == 'G'
		|| c == 't' || c == 'T';
}

bool	Lexer::is_path_special_char( char c ) const
{
	return c == '.' || c == '/';
}

void	Lexer::skip_comments( void )
{
	_config_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	advance();
	skip_whitespaces_and_comments();
}

void	Lexer::skip_whitespaces_and_comments( void )
{
	while (!reached_eof() && isspace(_current_char))
		advance();
	if (_current_char == '#')
		skip_comments();
}

bool	Lexer::verbose = false;
