/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:10:13 by pohl              #+#    #+#             */
/*   Updated: 2022/03/07 11:19:40 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/Token.hpp"

Token::Token( void ): _type(Token::error), _value("")
{
	if (Token::verbose)
		std::cout << "Default constructor for Token called" << std::endl;
	return;
}

Token::Token( Token const & src )
{
	if (Token::verbose)
		std::cout << "Copy constructor for Token called" << std::endl;
	*this = src;
	return;
}

Token::Token( token_type type, token_value value ): _type(type), _value(value)
{
	if (Token::verbose)
		std::cout << "Initialisation constructor for Token called with type: "
			<< this->_type << " and value: " << this->_value << std::endl;
	return;
}

Token::~Token( void )
{
	if (Token::verbose)
		std::cout << "Destructor for Token called" << std::endl;
	return;
}

Token &	Token::operator=( Token const& src )
{
	if (Token::verbose)
		std::cout << "Assignation operator for Token called" << std::endl;
	if (this == &src)
		return *this;
	this->_type = src.getType();
	this->_value = src.getValue();
	return *this;
}

int			Token::getType( void ) const { return this->_type; }

std::string Token::getValue( void ) const { return this->_value; }

bool	Token::operator==( const Token& other ) const
{
	return (other.getType() == this->getType()
			&& other.getValue() == this->getValue());
}
bool	Token::operator!=( const Token& other ) const
{
	return !(*this == other);
}

std::ostream &	operator<<( std::ostream & ostr, Token const & instance)
{
	ostr << "Token(" << instance.getType() << "): " << instance.getValue() << std::endl;
	return ostr;
}

bool	Token::verbose = true;
