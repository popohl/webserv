/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:55 by pohl              #+#    #+#             */
/*   Updated: 2022/03/30 18:20:56 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "configParsing/Rules.hpp"

class Cgi
{

public:

	Cgi( Rules& rules );
	~Cgi( void );

	std::string	executeCgi( void );

private:

	Cgi( void );
	Cgi( const Cgi &src );
	Cgi	&operator=( const Cgi &src );

	std::string	readCgiOutput( void );
	void	executeChildProcess( void );

	void	createArgv( const char* binPath, const char* filePath );
	void	writeToEnvp( std::map<std::string, std::string> envp);
	void	createEnvp( void );
	void	writeBodyToStdIn( void );
	int		createFork( void );
	bool	isChildProcess( int forkPid );
	void	createPipe( void );
	void	freeStringPointer( char** stringPtr );
	void	freeEnvp( void );
	void	freeArgv( void );

	Rules&	_rules;
	int		_pipeFd[2];
	char**	_envp;
	char**	_argv;

};

#endif
