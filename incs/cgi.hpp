/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:17 by pohl              #+#    #+#             */
/*   Updated: 2022/03/28 09:32:45 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>

std::string	executeCgi( void );
void		createPipe( int pipeFd[2] );
int			createFork( void );
bool		isChildProcess( int forkPid );

#endif
