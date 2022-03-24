/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:25:56 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 17:08:27 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include "configParsing/AST/ConfigFileNode.hpp"
# include "configParsing/Lexer.hpp"

class Parser
{

public:

	Parser( void );
	Parser( const std::string inputFileName );
	~Parser( void );

	void	openFile( const std::string inputFileName );

	const ConfigFileNode	&getConfigFile( void ) const;
	ConfigFileNode			&getConfigFile( void );

private:

	Parser( const Parser &src );
	Parser	&operator=( const Parser &src );

	void	eat( Token::tokenType expectedType );
	void	saveLocationTokens( std::vector<Token>& locationTokens );
	void	afterParseLocations( std::vector<Token> &locationTokens );
	Token	setIteratorModeAndSaveCurrent( std::vector<Token>& tokenVector );
	Token	setTokenModeAndRestoreCurrent( Token& svg );

	std::string createEatErrorMsg( Token::tokenType expectedType );

	void	parseConfigFile( void );
	void	parseServer( void );
	void	parseServerRule( void );
	void	parseLocation( void );
	void	parseLocationRule( void );

	void	parseAllowedMethod( LocationRules& locationRules );
	void	parseAutoindexRule( bool& autoindex );
	void	parseCgiRule( std::string& cgiExtension, std::string& cgiPath );
	void	parseClientMaxBodySizeRule( int& clientMaxBodySize );
	void	parseErrorPageRule( std::map<int, std::string>& errorPage );
	void	parseListenRule( ServerRules& serverRules );
	void	parseIndexRule( std::vector<std::string>& index );
	void	parseRedirectRule( LocationRules& locationRules );
	void	parseRootRule( std::string& root );
	void	parseServerNameRule( ServerRules& serverRules );
	void	parseUploadPathRule( std::string& uploadPath );

	ConfigFileNode						configFile;
	Lexer								lexer;
	Token								currentToken;
	std::vector<Token>::const_iterator	tokenIterator;
	bool								readLocationRules;

};

#endif
