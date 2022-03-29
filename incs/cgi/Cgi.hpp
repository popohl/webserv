/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:55 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 15:25:36 by pohl             ###   ########.fr       */
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

	std::string	readCgiOutput( int pipeFd[2] );
	void	executeChildProcess( const Rules &rules, int pipeFd[2] );

	char**	createArgv( const char* binPath, const char* filePath );
	char**	generateEnvpPtrFromMap( std::map<std::string, std::string> envp);
	char**	createEnvp( const Rules& rules );
	void	writeBodyToStdIn( void );
	int		createFork( void );
	bool	isChildProcess( int forkPid );
	void	createPipe( int pipeFd[2] );
	void	closePipe( int pipeFd[2] );

	Rules&	_rules;
	int		_pipeFd[2];

};

#endif
