/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:25:56 by pohl              #+#    #+#             */
/*   Updated: 2022/03/16 18:49:54 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include "config_parsing/AST/ConfigFileNode.hpp"
# include "config_parsing/Lexer.hpp"

class Parser
{

public:

	Parser( void );
	Parser( const std::string input_file_name );
	~Parser( void );

	void	open_file( const std::string input_file_name );

	const ConfigFileNode	&getConfigFile( void ) const;
	ConfigFileNode			&getConfigFile( void );

	static bool	verbose;

private:

	Parser( const Parser &src );
	Parser	&operator=( const Parser &src );

	void	eat( Token::token_type expectedType );
	void	saveLocationTokens( std::vector<Token>& locationTokens );
	void	afterParseLocations( std::vector<Token> &locationTokens );
	Token	setIteratorModeAndSaveCurrent( std::vector<Token>& tokenVector );
	Token	setTokenModeAndRestoreCurrent( Token& svg );

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
	Token								current_token;
	std::vector<Token>::const_iterator	tokenIterator;
	bool								readLocationRules;

};

#endif
