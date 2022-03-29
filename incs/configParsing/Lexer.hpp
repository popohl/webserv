/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:51:05 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 17:03:27 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include <iostream>
# include <fstream>
# include "configParsing/Token.hpp"

class Lexer
{

public:

	Lexer( void );
	Lexer( std::string fileName );
	~Lexer( void );

	Token	getNextToken( void );

	void	openFile( std::string fileName );
	char	advance( void );

private:

	void	closeFile( void );
	void	readFirstCharacter( void );

	bool	reachedEof( void ) const;
	bool	isPathSpecialChar( char c ) const;
	bool	isSizePrefix( char c ) const;
	void	skipWhitespacesAndComments( void );
	void	skipComments( void );

	std::string	getNumber( void );
	char		getSizeMultiplier( void );

	Token	tokenizeSingleCharacter( Token::tokenType type );
	Token	tokenizePath( void );
	Token	tokenizeWord( void );
	Token	differentiateDigitTokens( void );
	Token	differentiateNumberSize( std::string &beginning );
	Token	tokenizeSize( std::string &beginning );

	Lexer( const Lexer &src );
	Lexer	&operator=( const Lexer &rhs );

	std::ifstream	_configFile;
	char			_currentChar;

};

std::ostream	&operator<<( std::ostream &ostr, const Lexer &instance );

#endif
