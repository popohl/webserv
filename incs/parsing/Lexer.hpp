/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:51:05 by pohl              #+#    #+#             */
/*   Updated: 2022/03/07 12:47:56 by pohl             ###   ########.fr       */
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

private:

	void	open_file( std::string& file_name );

	Lexer( const Lexer &src );
	Lexer	&operator=( const Lexer &rhs );

	std::ifstream	_config_file;

};

std::ostream	&operator<<( std::ostream &ostr, const Lexer &instance );

#endif
