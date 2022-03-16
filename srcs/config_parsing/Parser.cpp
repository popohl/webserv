/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:25:53 by pohl              #+#    #+#             */
/*   Updated: 2022/03/16 19:51:07 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parsing/Parser.hpp"

Parser::Parser( void )
{
	if (Parser::verbose)
		std::cout << "Default constructor for Parser called" << std::endl;
	return;
}

Parser::Parser( const std::string input_file_name )
{
	if (Parser::verbose)
		std::cout << "input file constructor for Parser called" << std::endl;
	this->open_file(input_file_name);
	return;
}

Parser::~Parser( void )
{
	if (Parser::verbose)
		std::cout << "Destructor for Parser called" << std::endl;
	return;
}

void	Parser::eat( Token::token_type expectedType )
{
	if (this->current_token.getType() != expectedType)
		throw std::exception();
	if (this->readLocationRules)
	{
		this->current_token = *(++this->tokenIterator);
	}
	else
		this->current_token = lexer.get_next_token();
}

void	Parser::saveLocationTokens( std::vector<Token>& locationTokens )
{
	while (current_token.getType() != Token::closing_bracket)
	{
		locationTokens.push_back(current_token);
		this->current_token = lexer.get_next_token();
	}
	locationTokens.push_back(current_token);
	this->current_token = lexer.get_next_token();
}

Token	Parser::setIteratorModeAndSaveCurrent( std::vector<Token>& tokenVector )
{
	Token svg = current_token;

	tokenIterator = tokenVector.begin();
	current_token = *tokenIterator;
	readLocationRules = true;
	return svg;
}

Token	Parser::setTokenModeAndRestoreCurrent( Token& svg )
{
	readLocationRules = false;
	return svg;
}

void	Parser::open_file( const std::string input_file_name )
{
	lexer.open_file(input_file_name);
	this->current_token = this->lexer.get_next_token();
	this->parseConfigFile();
}

void	Parser::parseConfigFile( void )
{
	while (current_token.getType() != Token::end_of_file)
		parseServer();
}

void	Parser::afterParseLocations( std::vector<Token> &locationTokens )
{
	Token				svg;

	locationTokens.push_back(Token(Token::end_of_file, ""));
	svg = setIteratorModeAndSaveCurrent(locationTokens);
	while (current_token.getType() != Token::end_of_file)
		parseLocation();
	current_token = setTokenModeAndRestoreCurrent(svg);
}

void	Parser::parseServer( void )
{
	std::vector<Token>	locationTokens;

	if (current_token.getValue() != "server")
		throw std::exception();
	this->eat(Token::word);
	configFile.createNewServerNode();
	this->eat(Token::opening_bracket);
	while (current_token.getType() != Token::closing_bracket)
	{
		if (current_token.getValue() == "location")
			saveLocationTokens(locationTokens);
		else
			parseServerRule();
	}
	this->eat(Token::closing_bracket);
	if (!locationTokens.empty())
		afterParseLocations(locationTokens);
}

void	Parser::parseServerRule( void )
{
	std::string	ruleName = current_token.getValue();

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
	if (current_token.getValue() != "location")
		throw std::exception();
	configFile.LatestServer().createNewLocationNode();
	eat(Token::word);
	configFile.LatestServer().LatestLocation().locationPath
		= current_token.getValue();
	eat(Token::path);
	eat(Token::opening_bracket);
	while (current_token.getType() != Token::closing_bracket)
		parseLocationRule();
	eat(Token::closing_bracket);
}

void	Parser::parseLocationRule( void )
{
	std::string	ruleName = current_token.getValue();

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
	while (current_token.getType() == Token::word)
	{
		configFile.LatestServer().LatestLocation()
			.allow_method(current_token.getValue());
		eat(Token::word);
	}
}

void	Parser::parseAutoindexRule( void )
{
	std::string toggleValue = current_token.getValue();
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
	configFile.LatestServer().LatestLocation().cgi_extension
		= current_token.getValue();
	eat(Token::word);
}

void	Parser::parseCgiPathRule( void )
{
	configFile.LatestServer().LatestLocation().cgi_path
		= current_token.getValue();
	eat(Token::path);
}

void	Parser::parseClientMaxBodySizeRule( void )
{
	Token	sizeToken = current_token;

	if (current_token.getType() == Token::number)
		eat(Token::number);
	else
		eat(Token::size);
	if (readLocationRules)
		configFile.LatestServer().LatestLocation().client_max_body_size
			= sizeToken.size_atoi();
	else
		configFile.LatestServer().getServerRules().client_max_body_size
			= sizeToken.size_atoi();
}

void	Parser::parseErrorPageRule( void )
{
	int	errorCode = current_token.size_atoi();
	std::map<int, std::string>& errorPageRule = readLocationRules ?
		configFile.LatestServer().LatestLocation().error_page :
		configFile.LatestServer().getServerRules().error_page;

	eat(Token::number);
	errorPageRule[errorCode] = current_token.getValue();
	eat(Token::path);
}

void	Parser::parseListenRule( void )
{
	ServerRules&	serverRules = configFile.LatestServer().getServerRules();

	switch (current_token.getType())
	{
		case Token::ip_address:
			serverRules.listen_address = current_token.getValue();
			eat(Token::ip_address);
			if (current_token.getType() == Token::colon)
			{
				eat(Token::colon);
				serverRules.listen_port = current_token.size_atoi();
				eat(Token::number);
			}
			break;
		case Token::colon:
			eat(Token::colon);
		case Token::number:
			serverRules.listen_port = current_token.size_atoi();
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
	while (current_token.getType() == Token::word)
	{
		indexList.push_back(current_token.getValue());
		eat(Token::word);
	}
	if (current_token.getType() == Token::path)
	{
		indexList.push_back(current_token.getValue());
		eat(Token::path);
	}
	if (indexList.empty())
		throw std::exception();
}

void	Parser::parseRedirectRule( void )
{
	LocationRules&	locationRules = configFile.LatestServer().LatestLocation();

	if (current_token.getType() == Token::number)
	{
		locationRules.redirect_code = current_token.size_atoi();
		eat(Token::number);
	}
	else
		locationRules.redirect_code = 302;
	locationRules.redirect_uri = current_token.getValue();
	if (current_token.getType() == Token::word)
		eat(Token::word);
	else
		eat(Token::path);
}

void	Parser::parseRootRule( void )
{
	configFile.LatestServer().LatestLocation().root = current_token.getValue();
	eat(Token::path);
}

void	Parser::parseUploadPathRule( void )
{
	configFile.LatestServer().LatestLocation().upload_path = current_token.getValue();
	eat(Token::path);
}

void	Parser::parseServerNameRule( void )
{
	std::vector<std::string>	&serverNameList
		= configFile.LatestServer().getServerRules().server_name;

	serverNameList.clear();
	while (current_token.getType() == Token::word)
	{
		serverNameList.push_back(current_token.getValue());
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
