/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:10:13 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 12:31:29 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/Token.hpp"

Token::Token( void ): _type(Token::error), _value("")
{
	/* if (Token::verbose) */
	/* 	std::cout << "Default constructor for Token called" << std::endl; */
	return;
}

Token::Token( Token const & src )
{
	/* if (Token::verbose) */
	/* 	std::cout << "Copy constructor for Token called" << std::endl; */
	*this = src;
	return;
}

Token::Token( token_type type, token_value value ): _type(type), _value(value)
{
	if (Token::verbose)
		std::cout << "Initialisation constructor for " << *this << std::endl;
	return;
}

Token::~Token( void )
{
	/* if (Token::verbose) */
	/* 	std::cout << "Destructor for Token called" << std::endl; */
	return;
}

Token &	Token::operator=( Token const& src )
{
	/* if (Token::verbose) */
	/* 	std::cout << "Assignation operator for Token called" << std::endl; */
	if (this == &src)
		return *this;
	this->_type = src.getType();
	this->_value = src.getValue();
	return *this;
}

Token::token_type	Token::getType( void ) const { return this->_type; }

std::string		Token::getTypeName( void ) const
{
		if (this->_type == error)
			return std::string("error");
		if (this->_type == end_of_file)
			return std::string("end_of_file");
		if (this->_type == opening_bracket)
			return std::string("opening_bracket");
		if (this->_type == closing_bracket)
			return std::string("closing_bracket");
		if (this->_type == semicolon)
			return std::string("semicolon");
		if (this->_type == path)
			return std::string("path");
		if (this->_type == ip_address)
			return std::string("ip_address");
		if (this->_type == word)
			return std::string("word");
		if (this->_type == number)
			return std::string("number");
		return std::string("unknown token");
}

Token::token_value Token::getValue( void ) const { return this->_value; }

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
	ostr << "Token(" << instance.getTypeName() << "): " << instance.getValue();
	return ostr;
}

bool	Token::verbose = true;
