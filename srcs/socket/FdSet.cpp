/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdSet.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmonbeig <fmonbeig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:57:54 by fmonbeig          #+#    #+#             */
/*   Updated: 2022/03/23 11:30:10 by fmonbeig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FdSet.hpp"

// +------------------------------------------+ //
//   CANONICAL FORM 					        //
// +------------------------------------------+ //

FdSet::FdSet(void)
{	FD_ZERO(&_set);	}

FdSet::FdSet (const FdSet &other):
	_set(other._set)
{}

FdSet::~FdSet(void)
{}

FdSet &FdSet::operator=(const FdSet & rhs)
{
	if (this != &rhs)
	{
		_set = rhs._set;
	}
		return *this;
}

// +------------------------------------------+ //
//   MEMBER FUNCTION					        //
// +------------------------------------------+ //

fd_set	FdSet::getset()
{ return (_set); }

void	FdSet::add(int fd)
{ FD_SET(fd, &_set); }

void	FdSet::remove(int fd)
{ FD_CLR(fd, &_set); }

void	FdSet::clearAll()
{ FD_ZERO(&_set); }

bool	FdSet::isset(int fd)
{ return FD_ISSET(fd, &_set); }
