/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 09:29:34 by pohl              #+#    #+#             */
/*   Updated: 2022/03/28 10:15:32 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <unistd.h>

int	createFork( void )
{
	int forkPid = fork();

	if (forkPid == -1)
	{
		// 500 Internal Server Error
		throw std::exception();
	}
	return forkPid;
}

bool	isChildProcess( int forkPid )
{
	return (forkPid != 0);
}
