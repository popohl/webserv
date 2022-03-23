/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:25:56 by pohl              #+#    #+#             */
/*   Updated: 2022/03/22 15:38:32 by pohl             ###   ########.fr       */
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

	static bool	verbose;

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

	void	parseAllowedMethod( void );
	void	parseAutoindexRule( void );
	void	parseCgiExtensionRule( void );
	void	parseCgiPathRule( void );
	void	parseClientMaxBodySizeRule( void );
	void	parseErrorPageRule( void );
	void	parseListenRule( void );
	void	parseIndexRule( void );
	void	parseRedirectRule( void );
	void	parseRootRule( void );
	void	parseServerNameRule( void );
	void	parseUploadPathRule( void );

	ConfigFileNode						configFile;
	Lexer								lexer;
	Token								currentToken;
	std::vector<Token>::const_iterator	tokenIterator;
	bool								readLocationRules;

};

#endif
