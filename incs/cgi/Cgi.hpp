/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:31:17 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 15:31:24 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>

class Cgi
{

public:

	Cgi( void );
	Cgi( const Cgi &src );
	~Cgi( void );

	Cgi	&operator=( const Cgi &src );

	std::string executeCgi( void );

private:

};

std::ostream	&operator<<( std::ostream &ostr, const Cgi &instance );

#endif
