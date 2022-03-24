/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:21 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 15:31:23 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi/Cgi.hpp"

Cgi::Cgi( void )
{
	return;
}

Cgi::Cgi( Cgi const & src )
{
	*this = src;
	return;
}

Cgi::~Cgi( void )
{
	return;
}

Cgi &	Cgi::operator=( Cgi const & src )
{
	if (this == &src)
		return *this;
	// TODO: fill this
	return *this;
}
