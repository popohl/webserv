/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:10:13 by pohl              #+#    #+#             */
/*   Updated: 2022/03/15 10:42:21 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parsing/Token.hpp"

Token::Token( void ): _type(Token::end_of_file), _value("")
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
		std::cout << "Initialisation constructor for " << *this << std::endl;
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

Token::token_type	Token::getType( void ) const { return this->_type; }

std::string		Token::getTypeName( void ) const
{
		if (this->_type == end_of_file)
			return std::string("end_of_file");
		else if (this->_type == opening_bracket)
			return std::string("opening_bracket");
		else if (this->_type == closing_bracket)
			return std::string("closing_bracket");
		else if (this->_type == semicolon)
			return std::string("semicolon");
		else if (this->_type == colon)
			return std::string("colon");
		else if (this->_type == path)
			return std::string("path");
		else if (this->_type == ip_address)
			return std::string("ip_address");
		else if (this->_type == word)
			return std::string("word");
		else if (this->_type == number)
			return std::string("number");
		else if (this->_type == size)
			return std::string("size");
		return std::string("unknown token");
}

Token::token_value Token::getValue( void ) const { return this->_value; }

size_t		Token::get_size_multiplier( char c ) const
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

size_t		Token::size_atoi( void ) const
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
		multiplier = (i != _value.end()) ? get_size_multiplier(*i) : 1;
	else if (_type != Token::number)
		throw std::exception();
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
	ostr << "Token(" << instance.getTypeName() << "): " << instance.getValue();
	return ostr;
}

bool	Token::verbose = false;