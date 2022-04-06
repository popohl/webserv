/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpExceptions.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:21:31 by pohl              #+#    #+#             */
/*   Updated: 2022/04/06 11:17:52 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <stdexcept>

class httpError: public std::runtime_error {
	public:
		httpError( int errorStatusCode, const char* msg ):
			std::runtime_error(msg), errorStatusCode(errorStatusCode) { }
		int		statusCode( void ) { return errorStatusCode; }
	protected:
		int			errorStatusCode;
};

class clientError: public httpError
{
	public:
		clientError( int errorStatusCode, const char* msg ):
			httpError(errorStatusCode, msg) { }
		clientError( const char* msg ):
			httpError(400, msg) { }
};

class serverError: public httpError
{
	public:
		serverError( int errorStatusCode, const char* msg ):
			httpError(errorStatusCode, msg) { }
		serverError( const char* msg ):
			httpError(500, msg) { }
};

#endif
