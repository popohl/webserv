/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:10:11 by pohl              #+#    #+#             */
/*   Updated: 2022/03/18 12:30:01 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>

class Token
{

public:

	typedef	int			tokenType;
	typedef	std::string tokenValue;

	enum	Types
	{
		endOfFile = 0,
		openingBracket,
		closingBracket,
		semicolon,
		colon,
		path,
		ipAddress,
		size,
		word,
		number,
	};

	Token( void );
	Token( const Token &src );
	Token( tokenType type, tokenValue value );
	~Token( void );

	Token	&operator=( const Token &src );

	tokenType	getType( void ) const;
	std::string getTypeName( void ) const;
	tokenValue getValue( void ) const;

	size_t		sizeAtoi( void ) const;

	bool	operator==( const Token& other ) const;
	bool	operator!=( const Token& other ) const;

	static bool	verbose;

private:

	size_t		getSizeMultiplier( char c ) const;

	tokenType	_type;
	tokenValue	_value;

};

std::ostream	&operator<<( std::ostream &ostr, const Token &instance );

#endif
