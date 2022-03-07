/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:10:11 by pohl              #+#    #+#             */
/*   Updated: 2022/03/07 12:46:55 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <iostream>

class Token
{

public:

	typedef	int			token_type;
	typedef	std::string token_value;

	enum	Types
	{
		error = -1,
		end_of_file = 0,
		obracket,
		cbracket,
		semicolon,
		dot,
		slash,
		on,
		off,
		name,
		ext
	};

	Token( void );
	Token( const Token &src );
	Token( token_type type, token_value value );
	~Token( void );

	Token	&operator=( const Token &src );

	token_type	getType( void ) const;
	token_value getValue( void ) const;

	bool	operator==( const Token& other ) const;
	bool	operator!=( const Token& other ) const;

	static bool	verbose;

private:

	token_type	_type;
	token_value _value;

};

std::ostream	&operator<<( std::ostream &ostr, const Token &instance );

#endif
