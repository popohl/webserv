/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:17 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 13:37:42 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include "configParsing/Rules.hpp"

std::string	executeCgi( const Rules &rules );

/*
 * pipe.cpp
 */
void		createPipe( int pipeFd[2] );
void		closePipe( int pipeFd[2] );

/*
 * fork.cpp
 */
int			createFork( void );
bool		isChildProcess( int forkPid );

/*
 * childProcess.cpp
 */
void	executeChildProcess( const Rules &rules, int pipeFd[2] );

#endif
