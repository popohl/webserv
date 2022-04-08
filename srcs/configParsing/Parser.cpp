/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:25:53 by pohl              #+#    #+#             */
/*   Updated: 2022/04/08 11:25:11 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "configParsing/Parser.hpp"
#include "configParsing/Exception.hpp"

Parser::Parser( void ): readLocationRules(false) { return; }

Parser::Parser( const std::string inputFileName ): readLocationRules(false)
{
	this->openFile(inputFileName);
	return;
}

Parser::~Parser( void ) { return; }

void	Parser::eat( Token::tokenType expectedType )
{
	if (this->currentToken.getType() != expectedType)
		throw ParsingException(createEatErrorMsg(expectedType).c_str());
	if (this->readLocationRules)
		this->currentToken = *(++this->tokenIterator);
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

std::string	Parser::createEatErrorMsg( Token::tokenType expectedType )
{
	std::stringstream	out;

	out << "Invalid element found at: " << currentToken << std::endl;
	out << "           server number: " << configFile.getServerList().size()
		<< std::endl;
	if (this->readLocationRules)
		out << "           location number: "
			<< configFile.latestServer().getLocationList().size() << std::endl;
	out << "           expected: " << Token(expectedType, "").getTypeName()
		<< std::endl;
	return out.str();
}

void	Parser::openFile( const std::string inputFileName )
{
	lexer.openFile(inputFileName);
	this->currentToken = this->lexer.getNextToken();
	this->parseConfigFile();
}

void	Parser::theCorrectionIsTrash( void )
{
	std::vector<ServerNode>::iterator it1, it2;

	for (it1 = configFile.getServerList().begin();
			it1 != configFile.getServerList().end(); it1++)
	{
		for (it2 = it1 + 1; it2 != configFile.getServerList().end(); it2++)
		{
			if (it1->getServerRules().listenPort == it2->getServerRules().listenPort)
				throw ParsingException("Multiple servers listening on the same "
						"port is not allowed");
		}
	}
}

void	Parser::parseConfigFile( void )
{
	while (currentToken.getType() != Token::endOfFile)
		parseServer();
	/* theCorrectionIsTrash(); */
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
		throw ParsingException("Expected \"server\" keyword");
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
	ServerRules& serverRules = configFile.latestServer().getServerRules();

	eat(Token::word);
	if (ruleName == "autoindex")
		parseAutoindexRule(serverRules.autoindex);
	else if (ruleName == "client_max_body_size")
		parseClientMaxBodySizeRule(serverRules.clientMaxBodySize);
	else if (ruleName == "cgi")
		parseCgiRule(serverRules.cgiExtension, serverRules.cgiPath);
	else if (ruleName == "error_page")
		parseErrorPageRule(serverRules.errorPage);
	else if (ruleName == "index")
		 parseIndexRule(serverRules.index);
	else if (ruleName == "listen")
		parseListenRule(serverRules);
	else if (ruleName == "server_name")
		parseServerNameRule(serverRules);
	else
	{
		std::string error = "rule name " + ruleName + " invalid";
		throw ParsingException(error.c_str());
	}
	eat(Token::semicolon);
}

void	Parser::parseLocation( void )
{
	if (currentToken.getValue() != "location")
		throw ParsingException("Unknown keyword encountered");
	configFile.latestServer().createNewLocationNode();
	eat(Token::word);
	configFile.latestServer().latestLocation().locationPath
		= currentToken.getValue();
	if (*(configFile.latestServer().latestLocation().locationPath.rbegin()) != '/')
		configFile.latestServer().latestLocation().locationPath.push_back('/');
	eat(Token::path);
	eat(Token::openingBracket);
	while (currentToken.getType() != Token::closingBracket)
		parseLocationRule();
	eat(Token::closingBracket);
}

void	Parser::parseLocationRule( void )
{
	std::string	ruleName = currentToken.getValue();
	LocationRules& locationRules = configFile.latestServer().latestLocation();

	eat(Token::word);
	if (ruleName == "allowed_method")
		parseAllowedMethod(locationRules);
	else if (ruleName == "autoindex")
		parseAutoindexRule(locationRules.autoindex);
	else if (ruleName == "cgi")
		parseCgiRule(locationRules.cgiExtension, locationRules.cgiPath);
	else if (ruleName == "client_max_body_size")
		parseClientMaxBodySizeRule(locationRules.clientMaxBodySize);
	else if (ruleName == "error_page")
		parseErrorPageRule(locationRules.errorPage);
	else if (ruleName == "index")
		parseIndexRule(locationRules.index);
	else if (ruleName == "redirect")
		parseRedirectRule(locationRules);
	else if (ruleName == "root")
		parseRootRule(locationRules.root);
	else if (ruleName == "upload_path")
		parseUploadPathRule(locationRules.uploadPath);
	else
	{
		std::string error = "rule name " + ruleName + " invalid";
		throw ParsingException(error.c_str());
	}
	eat(Token::semicolon);
}

void	Parser::parseAllowedMethod( LocationRules& locationRules )
{
	locationRules.forbidMethod(LocationRules::ALL_METHODS);
	while (currentToken.getType() == Token::word)
	{
		locationRules.allowMethod(currentToken.getValue());
		eat(Token::word);
	}
}

void	Parser::parseAutoindexRule( bool& autoindex )
{
	std::string toggleValue = currentToken.getValue();

	eat(Token::word);
	if (toggleValue == "on")
		autoindex = true;
	else if (toggleValue == "off")
		autoindex = false;
	else
		throw ParsingException("autoindex value invalid");
}

void	Parser::parseCgiRule( std::string& cgiExtension, std::string& cgiPath )
{
	if (currentToken.getType() == Token::word)
	{
		cgiExtension = currentToken.getValue();
		eat(Token::word);
		cgiPath = currentToken.getValue();
		eat(Token::path);
	}
	else
	{
		cgiPath = currentToken.getValue();
		eat(Token::path);
		cgiExtension = currentToken.getValue();
		eat(Token::word);
	}
}

void	Parser::parseClientMaxBodySizeRule( int& clientMaxBodySize )
{
	Token	sizeToken = currentToken;

	if (currentToken.getType() == Token::number)
		eat(Token::number);
	else
		eat(Token::size);
	clientMaxBodySize = sizeToken.sizeAtoi();
}

void	Parser::parseErrorPageRule( std::map<int, std::string>& errorPage )
{
	int	errorCode = currentToken.sizeAtoi();

	eat(Token::number);
	errorPage[errorCode] = currentToken.getValue();
	eat(Token::path);
}

void	Parser::parseListenRule( ServerRules& serverRules )
{
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
			/* fall through */
		case Token::number:
			serverRules.listenPort = currentToken.sizeAtoi();
			eat(Token::number);
			break;
		default:
			throw ParsingException(
						createEatErrorMsg(Token::ipAddress).c_str()
					);
	}
}

void	Parser::parseIndexRule( std::vector<std::string>& index )
{
	index.clear();
	while (currentToken.getType() == Token::word)
	{
		index.push_back(currentToken.getValue());
		eat(Token::word);
	}
	if (currentToken.getType() == Token::path)
	{
		index.push_back(currentToken.getValue());
		eat(Token::path);
	}
	if (index.empty())
		throw ParsingException("index rule cannot be empty");
}

void	Parser::parseRedirectRule( LocationRules& locationRules )
{
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

void	Parser::parseRootRule( std::string& root )
{
	root = currentToken.getValue();
	if (*(root.rbegin()) != '/')
		root.push_back('/');
	eat(Token::path);
}

void	Parser::parseServerNameRule( ServerRules& serverRules )
{
	serverRules.serverName.clear();
	while (currentToken.getType() == Token::word)
	{
		serverRules.addServerName(currentToken.getValue());
		eat(Token::word);
	}
	if (serverRules.serverName.empty())
		throw ParsingException("server_name rule cannot be empty");
}

void	Parser::parseUploadPathRule( std::string& uploadPath )
{
	uploadPath = currentToken.getValue();
	eat(Token::path);
}

const ConfigFileNode	&Parser::getConfigFile( void ) const
{
	return this->configFile;
}

ConfigFileNode			&Parser::getConfigFile( void )
{
	return this->configFile;
}
