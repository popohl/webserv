/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:25:53 by pohl              #+#    #+#             */
/*   Updated: 2022/03/18 12:45:11 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/Parser.hpp"

Parser::Parser( void )
{
	if (Parser::verbose)
		std::cout << "Default constructor for Parser called" << std::endl;
	return;
}

Parser::Parser( const std::string inputFileName )
{
	if (Parser::verbose)
		std::cout << "input file constructor for Parser called" << std::endl;
	this->openFile(inputFileName);
	return;
}

Parser::~Parser( void )
{
	if (Parser::verbose)
		std::cout << "Destructor for Parser called" << std::endl;
	return;
}

void	Parser::eat( Token::tokenType expectedType )
{
	if (this->currentToken.getType() != expectedType)
		throw std::exception();
	if (this->readLocationRules)
	{
		this->currentToken = *(++this->tokenIterator);
	}
	else
		this->currentToken = lexer.getNextToken();
}

void	Parser::saveLocationTokens( std::vector<Token>& locationTokens )
{
	while (currentToken.getType() != Token::closingBracket)
	{
		locationTokens.push_back(currentToken);
		this->currentToken = lexer.getNextToken();
	}
	locationTokens.push_back(currentToken);
	this->currentToken = lexer.getNextToken();
}

Token	Parser::setIteratorModeAndSaveCurrent( std::vector<Token>& tokenVector )
{
	Token svg = currentToken;

	tokenIterator = tokenVector.begin();
	currentToken = *tokenIterator;
	readLocationRules = true;
	return svg;
}

Token	Parser::setTokenModeAndRestoreCurrent( Token& svg )
{
	readLocationRules = false;
	return svg;
}

void	Parser::openFile( const std::string inputFileName )
{
	lexer.openFile(inputFileName);
	this->currentToken = this->lexer.getNextToken();
	this->parseConfigFile();
}

void	Parser::checkCgiValidity( void )
{
	LocationRules	&locationRules = configFile.LatestServer().LatestLocation();

	if (locationRules.cgiExtension != "" && locationRules.cgiPath == "")
		throw std::exception();
}

void	Parser::parseConfigFile( void )
{
	while (currentToken.getType() != Token::endOfFile)
		parseServer();
}

void	Parser::afterParseLocations( std::vector<Token> &locationTokens )
{
	Token				svg;

	locationTokens.push_back(Token(Token::endOfFile, ""));
	svg = setIteratorModeAndSaveCurrent(locationTokens);
	while (currentToken.getType() != Token::endOfFile)
		parseLocation();
	currentToken = setTokenModeAndRestoreCurrent(svg);
}

void	Parser::parseServer( void )
{
	std::vector<Token>	locationTokens;

	if (currentToken.getValue() != "server")
		throw std::exception();
	this->eat(Token::word);
	configFile.createNewServerNode();
	this->eat(Token::openingBracket);
	while (currentToken.getType() != Token::closingBracket)
	{
		if (currentToken.getValue() == "location")
			saveLocationTokens(locationTokens);
		else
			parseServerRule();
	}
	this->eat(Token::closingBracket);
	if (!locationTokens.empty())
		afterParseLocations(locationTokens);
}

void	Parser::parseServerRule( void )
{
	std::string	ruleName = currentToken.getValue();

	eat(Token::word);
	if (ruleName == "autoindex")
		parseAutoindexRule();
	else if (ruleName == "client_max_body_size")
		parseClientMaxBodySizeRule();
	else if (ruleName == "error_page")
		parseErrorPageRule();
	else if (ruleName == "index")
		 parseIndexRule();
	else if (ruleName == "listen")
		parseListenRule();
	else if (ruleName == "server_name")
		parseServerNameRule();
	else
		throw std::exception(); // rule name invalid
	eat(Token::semicolon);
}

void	Parser::parseLocation( void )
{
	if (currentToken.getValue() != "location")
		throw std::exception();
	configFile.LatestServer().createNewLocationNode();
	eat(Token::word);
	configFile.LatestServer().LatestLocation().locationPath
		= currentToken.getValue();
	eat(Token::path);
	eat(Token::openingBracket);
	while (currentToken.getType() != Token::closingBracket)
		parseLocationRule();
	eat(Token::closingBracket);
	checkCgiValidity();
}

void	Parser::parseLocationRule( void )
{
	std::string	ruleName = currentToken.getValue();

	eat(Token::word);
	if (ruleName == "allowed_method")
		parseAllowedMethod();
	else if (ruleName == "autoindex")
		parseAutoindexRule();
	else if (ruleName == "cgi_extension")
		parseCgiExtensionRule();
	else if (ruleName == "cgi_path")
		parseCgiPathRule();
	else if (ruleName == "client_max_body_size")
		parseClientMaxBodySizeRule();
	else if (ruleName == "error_page")
		parseErrorPageRule();
	else if (ruleName == "index")
		parseIndexRule();
	else if (ruleName == "redirect")
		parseRedirectRule();
	else if (ruleName == "root")
		parseRootRule();
	else if (ruleName == "upload_path")
		parseUploadPathRule();
	else
		throw std::exception(); // rule name invalid
	eat(Token::semicolon);
}

void	Parser::parseAllowedMethod( void )
{
	configFile.LatestServer().LatestLocation()
		.forbidMethod(LocationRules::ALL_METHODS);
	while (currentToken.getType() == Token::word)
	{
		configFile.LatestServer().LatestLocation()
			.allowMethod(currentToken.getValue());
		eat(Token::word);
	}
}

void	Parser::parseAutoindexRule( void )
{
	std::string toggleValue = currentToken.getValue();
	bool&	autoindex = readLocationRules
		? configFile.LatestServer().LatestLocation().autoindex
		: configFile.LatestServer().getServerRules().autoindex;

	eat(Token::word);
	if (toggleValue == "on")
		autoindex = true;
	else if (toggleValue == "off")
		autoindex = false;
	else
		throw std::exception(); // autoindex value invalid
}

void	Parser::parseCgiExtensionRule( void )
{
	configFile.LatestServer().LatestLocation().cgiExtension
		= currentToken.getValue();
	eat(Token::word);
}

void	Parser::parseCgiPathRule( void )
{
	configFile.LatestServer().LatestLocation().cgiPath
		= currentToken.getValue();
	eat(Token::path);
}

void	Parser::parseClientMaxBodySizeRule( void )
{
	Token	sizeToken = currentToken;

	if (currentToken.getType() == Token::number)
		eat(Token::number);
	else
		eat(Token::size);
	if (readLocationRules)
		configFile.LatestServer().LatestLocation().clientMaxBodySize
			= sizeToken.sizeAtoi();
	else
		configFile.LatestServer().getServerRules().clientMaxBodySize
			= sizeToken.sizeAtoi();
}

void	Parser::parseErrorPageRule( void )
{
	int	errorCode = currentToken.sizeAtoi();
	std::map<int, std::string>& errorPageRule = readLocationRules ?
		configFile.LatestServer().LatestLocation().errorPage :
		configFile.LatestServer().getServerRules().errorPage;

	eat(Token::number);
	errorPageRule[errorCode] = currentToken.getValue();
	eat(Token::path);
}

void	Parser::parseListenRule( void )
{
	ServerRules&	serverRules = configFile.LatestServer().getServerRules();

	switch (currentToken.getType())
	{
		case Token::ipAddress:
			serverRules.listenAddress = currentToken.getValue();
			eat(Token::ipAddress);
			if (currentToken.getType() == Token::colon)
			{
				eat(Token::colon);
				serverRules.listenPort = currentToken.sizeAtoi();
				eat(Token::number);
			}
			break;
		case Token::colon:
			eat(Token::colon);
		case Token::number:
			serverRules.listenPort = currentToken.sizeAtoi();
			eat(Token::number);
			break;
		default:
			throw std::exception();
	}
}

void	Parser::parseIndexRule( void )
{
	std::vector<std::string>&	indexList = readLocationRules ?
		configFile.LatestServer().LatestLocation().index :
		configFile.LatestServer().getServerRules().index;

	indexList.clear();
	while (currentToken.getType() == Token::word)
	{
		indexList.push_back(currentToken.getValue());
		eat(Token::word);
	}
	if (currentToken.getType() == Token::path)
	{
		indexList.push_back(currentToken.getValue());
		eat(Token::path);
	}
	if (indexList.empty())
		throw std::exception();
}

void	Parser::parseRedirectRule( void )
{
	LocationRules&	locationRules = configFile.LatestServer().LatestLocation();

	if (currentToken.getType() == Token::number)
	{
		locationRules.redirectCode = currentToken.sizeAtoi();
		eat(Token::number);
	}
	else
		locationRules.redirectCode = 302;
	locationRules.redirectUri = currentToken.getValue();
	if (currentToken.getType() == Token::word)
		eat(Token::word);
	else
		eat(Token::path);
}

void	Parser::parseRootRule( void )
{
	configFile.LatestServer().LatestLocation().root = currentToken.getValue();
	eat(Token::path);
}

void	Parser::parseUploadPathRule( void )
{
	configFile.LatestServer().LatestLocation().uploadPath = currentToken.getValue();
	eat(Token::path);
}

void	Parser::parseServerNameRule( void )
{
	std::vector<std::string>	&serverNameList
		= configFile.LatestServer().getServerRules().serverName;

	serverNameList.clear();
	while (currentToken.getType() == Token::word)
	{
		serverNameList.push_back(currentToken.getValue());
		eat(Token::word);
	}
	if (serverNameList.empty())
		throw std::exception();
}

const ConfigFileNode	&Parser::getConfigFile( void ) const
{
	return this->configFile;
}

ConfigFileNode			&Parser::getConfigFile( void )
{
	return this->configFile;
}

bool	Parser::verbose = false;
