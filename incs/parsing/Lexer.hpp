/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:51:05 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 14:29:12 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include <iostream>
# include <fstream>
# include "parsing/Token.hpp"

class Lexer
{

public:

	Lexer( void );
	Lexer( std::string& file_name );
	~Lexer( void );

	Token	get_next_token( void );

	void	open_file( std::string& file_name );
	char	advance( void );

	static bool	verbose;

public:

	class file_exception: public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("Couldn't read config file");
		}
	};
	class token_exception: public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("Invalid character found");
		}
	};

private:

	void	close_file( void );
	void	read_first_character( void );

	bool	reached_eof( void ) const;
	bool	is_path_special_char( char c ) const;
	void	skip_whitespaces_and_comments( void );
	void	request_whitespace( void ) const;
	void	skip_comments( void );

	std::string	get_number( void );

	Token	tokenize_single_character( Token::token_type type );
	Token	tokenize_path( void );
	Token	tokenize_word( void );
	Token	tokenize_number_or_ip( void );
	Token	tokenize_ip( std::string first_number );

	Lexer( const Lexer &src );
	Lexer	&operator=( const Lexer &rhs );

	std::ifstream	_config_file;
	char			_current_char;

};

std::ostream	&operator<<( std::ostream &ostr, const Lexer &instance );

#endif
