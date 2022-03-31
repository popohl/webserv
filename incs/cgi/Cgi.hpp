/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 14:51:55 by pohl              #+#    #+#             */
/*   Updated: 2022/03/31 12:42:36 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <cstdio>
# include "configParsing/Rules.hpp"

# define PIPE_WRITE 1
# define PIPE_READ 0

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
	void		executeChildProcess( void );

	char**	createArgv( const char* binPath, const char* filePath );
	char**	generateEnvpPtrFromMap( std::map<std::string, std::string> envp);
	char**	createEnvp( void );
	void	writeBodyToStdIn( void );
	int		createFork( void );
	bool	isChildProcess( int forkPid );
	void	createPipe( int pipeFd[2] );

	Rules&	_rules;
	int		_pipeFd[2];

};

#endif
