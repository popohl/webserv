/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:17 by pohl              #+#    #+#             */
/*   Updated: 2022/03/29 10:16:11 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>

/*
 * pipe.cpp
 */
void		createPipe( int pipeFd[2] );
void		closePipe( int pipeFd[2] );

/*
 * fork.cpp
 */
std::string	executeCgi( void );
int			createFork( void );
bool		isChildProcess( int forkPid );

/*
 * childProcess.cpp
 */
void	executeChildProcess( int pipeFd[2] );

#endif
