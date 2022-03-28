/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:10:13 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 17:03:27 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/Token.hpp"
#include "configParsing/Exception.hpp"

Token::Token( void ): _type(Token::endOfFile), _value("")
{
	return;
}

Token::Token( Token const & src )
{
	*this = src;
	return;
}

Token::Token( tokenType type, tokenValue value ): _type(type), _value(value)
{
	return;
}

Token::~Token( void )
{
	return;
}

Token &	Token::operator=( Token const& src )
{
	if (this == &src)
		return *this;
	this->_type = src.getType();
	this->_value = src.getValue();
	return *this;
}

Token::tokenType	Token::getType( void ) const { return this->_type; }

std::string		Token::getTypeName( void ) const
{
		if (this->_type == endOfFile)
			return std::string("endOfFile");
		else if (this->_type == openingBracket)
			return std::string("openingBracket");
		else if (this->_type == closingBracket)
			return std::string("closingBracket");
		else if (this->_type == semicolon)
			return std::string("semicolon");
		else if (this->_type == colon)
			return std::string("colon");
		else if (this->_type == path)
			return std::string("path");
		else if (this->_type == ipAddress)
			return std::string("ipAddress");
		else if (this->_type == word)
			return std::string("word");
		else if (this->_type == number)
			return std::string("number");
		else if (this->_type == size)
			return std::string("size");
		return std::string("unknown token");
}

Token::tokenValue Token::getValue( void ) const { return this->_value; }

std::string	Token::toStr( void ) const
{
	return std::string("Token(" + getTypeName() + "): " + getValue());
}

size_t		Token::getSizeMultiplier( char c ) const
{
	size_t	multiplier = 1;

	if (c == 'K')
		multiplier = 1024;
	else if (c == 'M')
		multiplier = 1048576;
	else if (c == 'G')
		multiplier = 1073741824;
	else if (c == 'T')
		multiplier = 1099511627776;
	return multiplier;
}

size_t		Token::sizeAtoi( void ) const
{
	size_t						result = 0;
	size_t						multiplier = 1;
	std::string::const_iterator	i = _value.begin();

	while (i != _value.end() && *i >= '0' && *i <= '9')
	{
		result *= 10;
		result += *i - 48;
		i++;
	}
	if (_type == Token::size )
		multiplier = (i != _value.end()) ? getSizeMultiplier(*i) : 1;
	else if (_type != Token::number)
		throw ParsingException("Invalid element found, tried to convert a non "
				"numeric value into an int");
	return result * multiplier;
}


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
	ostr << instance.toStr();
	return ostr;
}
