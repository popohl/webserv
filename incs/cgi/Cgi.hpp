/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:55 by pohl              #+#    #+#             */
/*   Updated: 2022/04/06 16:35:14 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <cstdio>
# include "configParsing/Rules.hpp"
# include "requests/requests.hpp"

# define PIPE_WRITE 1
# define PIPE_READ 0

class Cgi
{

private:

	typedef std::map<std::string, std::string> string_map;

public:

	Cgi( Rules& rules, iRequest* request );
	~Cgi( void );

	void		executeCgi( std::string requestedFilePath );
	int			parseAndRemoveHeaders( response& response );
	std::string writeBodyToTmpFile( void );

private:

	Cgi( void );
	Cgi( const Cgi &src );
	Cgi	&operator=( const Cgi &src );

	void	readCgiOutput( void );
	void	executeChildProcess( std::string requestedFilePath );

	void	createArgv( const char* binPath, const char* filePath );
	void	writeToEnvp( const std::map<std::string, std::string>& envp);
	void	createEnvp( std::string requestedFilePath );
	void	writeBodyToStdIn( void );
	int		createFork( void );
	bool	isChildProcess( int forkPid );
	void	createPipe( int pipeFd[2] );
	void	freeStringPointer( char** stringPtr );
	void	freeEnvp( void );
	void	freeArgv( void );
	bool	isPostRequest( void );
	void	setFromHeader( const char* envpKey, string_map& envp,
		const char* headerKey, const string_map& header );
	void	setPathInfo( std::string& requestedFilePath, string_map& envp );
	int		writeHeadersToResponse( std::string& rawHdrs, response& rspnse);

	const char*	stripExtraPathInfo( std::string& requestedFilePath );
	std::string popHeadersFromCgiOutput( void );

	Rules&			_rules;
	int				_pipeFd[2];
	iRequest*	_request;
	char**			_envp;
	char**			_argv;
	std::string		_rawCgiOutput;
	int				_status;

};

#endif
