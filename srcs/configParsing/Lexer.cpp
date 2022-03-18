/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 10:00:33 by pohl              #+#    #+#             */
/*   Updated: 2022/03/18 12:18:00 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/Lexer.hpp"
#include <limits>
#include <cctype>

Lexer::Lexer( void )
{
	if (Lexer::verbose)
		std::cout << "Empty constructor for Lexer called" << std::endl;
	return;
}

Lexer::Lexer( std::string fileName )
{
	if (Lexer::verbose)
		std::cout << "Standard constructor for Lexer called" << std::endl;
	this->openFile(fileName);
}

Lexer::~Lexer( void )
{
	if (Lexer::verbose)
		std::cout << "Destructor for Lexer called" << std::endl;
	this->closeFile();
	return;
}

Token	Lexer::getNextToken( void )
{
	skipWhitespacesAndComments();
	if (reachedEof())
		return tokenizeSingleCharacter(Token::endOfFile);
	if (_currentChar == '{')
		return tokenizeSingleCharacter(Token::openingBracket);
	if (_currentChar == '}')
		return tokenizeSingleCharacter(Token::closingBracket);
	if (_currentChar == ';')
		return tokenizeSingleCharacter(Token::semicolon);
	if (_currentChar == ':')
		return tokenizeSingleCharacter(Token::colon);
	if (isPathSpecialChar(_currentChar))
		return tokenizePath();
	if (isalpha(_currentChar))
		return tokenizeWord();
	if (isdigit(_currentChar))

		return differentiateDigitTokens();
	throw Lexer::tokenException();
}

Token	Lexer::tokenizeSingleCharacter( Token::tokenType type )
{
	advance();
	return Token(type, "");
}

Token	Lexer::tokenizeWord( void )
{
	std::string	word;

	do
	{
		word += advance();
	} while (isalnum(_currentChar) || _currentChar == '.' || _currentChar == '_');
	return Token(Token::word, word);
}

Token	Lexer::differentiateDigitTokens( void )
{
	std::string	number;

	number += getNumber();
	if (_currentChar == '.')
		return differentiateNumberSize(number);
	else if (isSizePrefix(_currentChar) || _currentChar == 'b'
			|| _currentChar == 'B')
		return tokenizeSize(number);
	return Token(Token::number, number);
}

Token	Lexer::differentiateNumberSize( std::string &beginning )
{
	std::string	ip(beginning);

	if (_currentChar == '.')
		ip += advance();
	else
		throw Lexer::tokenException();
	ip += getNumber();
	if (_currentChar == '.')
		ip += advance();
	else
		return tokenizeSize(ip);
	ip += getNumber();
	if (_currentChar == '.')
		ip += advance();
	else
		throw Lexer::tokenException();
	ip += getNumber();
	return Token(Token::ipAddress, ip);
}

Token	Lexer::tokenizeSize( std::string &numberPart )
{
	std::string	size(numberPart);

	if (isSizePrefix(_currentChar))
		size += getSizeMultiplier();
	else if (_currentChar == 'b' || _currentChar == 'B')
	{
		advance();
		return Token(Token::size, size);
	}
	else
		throw Lexer::tokenException();
	if (_currentChar == 'i')
		advance();
	if (_currentChar == 'b' || _currentChar == 'B')
		advance();
	return Token(Token::size, size);
}

char	Lexer::getSizeMultiplier( void )
{
	char		tmpUppercase;

	tmpUppercase = advance();
	if (tmpUppercase == 'k' || tmpUppercase == 'm' || tmpUppercase == 'g'
			|| tmpUppercase == 't')
		tmpUppercase -= 32;
	return tmpUppercase;
}

Token	Lexer::tokenizePath( void )
{
	std::string	path;

	do
	{
		path += advance();
	} while (isalnum(_currentChar) || isPathSpecialChar(_currentChar));
	return Token(Token::path, path);
}

std::string	Lexer::getNumber( void )
{
	std::string	number;

	while (isdigit(_currentChar))
		number += advance();
	return number;
}

void	Lexer::openFile( std::string fileName )
{
	this->closeFile();
	if (Lexer::verbose)
		std::cout << "Opening file: " << fileName << std::endl;
	this->_configFile.open(fileName.c_str());
	if (this->_configFile.fail())
		throw Lexer::fileException();
	else if (Lexer::verbose)
		std::cout << "Success opening file" << std::endl;
	readFirstCharacter();
}

void	Lexer::closeFile( void )
{
	if (Lexer::verbose)
		std::cout << "closing current file" << std::endl;
	if (this->_configFile.is_open())
		this->_configFile.close();
}

char	Lexer::advance( void )
{
	char tmp = _currentChar;
	_currentChar = _configFile.get();
	return tmp;
}

void	Lexer::readFirstCharacter( void )
{
	advance();
}

bool	Lexer::reachedEof( void ) const
{
	return _configFile.eof();
}

bool	Lexer::isSizePrefix( char c ) const
{
	return c == 'm' || c == 'M' || c == 'k' || c == 'K' || c == 'g' || c == 'G'
		|| c == 't' || c == 'T';
}

bool	Lexer::isPathSpecialChar( char c ) const
{
	return c == '.' || c == '/';
}

void	Lexer::skipComments( void )
{
	_configFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	advance();
	skipWhitespacesAndComments();
}

void	Lexer::skipWhitespacesAndComments( void )
{
	while (!reachedEof() && isspace(_currentChar))
		advance();
	if (_currentChar == '#')
		skipComments();
}

bool	Lexer::verbose = false;

