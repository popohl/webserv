/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:55 by pohl              #+#    #+#             */
/*   Updated: 2022/04/01 16:43:07 by pohl             ###   ########.fr       */
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

public:

	Cgi( Rules& rules, const iRequest* request );
	~Cgi( void );

	std::string	executeCgi( std::string requestedFilePath );

private:

	Cgi( void );
	Cgi( const Cgi &src );
	Cgi	&operator=( const Cgi &src );

	std::string	readCgiOutput( void );
	void	executeChildProcess( std::string requestedFilePath );

	void	createArgv( const char* binPath, const char* filePath );
	void	writeToEnvp( std::map<std::string, std::string> envp);
	void	createEnvp( std::string requestedFilePath );
	void	writeBodyToStdIn( void );
	int		createFork( void );
	bool	isChildProcess( int forkPid );
	void	createPipe( int pipeFd[2] );
	void	freeStringPointer( char** stringPtr );
	void	freeEnvp( void );
	void	freeArgv( void );
	bool	isPostRequest( void );
	const char*	stripExtraPathInfo( std::string& requestedFilePath );

	Rules&			_rules;
	int				_pipeFd[2];
	const iRequest*	_request;
	char**			_envp;
	char**			_argv;

};

#endif
