/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpExceptions.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <paul.lv.ohl@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:21:31 by pohl              #+#    #+#             */
/*   Updated: 2022/04/06 18:14:03 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <stdexcept>

class httpError: public std::runtime_error {
	public:
		httpError( int errorStatusCode, const char* msg ):
			std::runtime_error(msg), errorStatusCode(errorStatusCode) { }
		httpError( int errorStatusCode ):
			std::runtime_error(""), errorStatusCode(errorStatusCode) { }
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
		clientError( int statusCode ):
			httpError(statusCode) { }
};

class serverError: public httpError
{
	public:
		serverError( int errorStatusCode, const char* msg ):
			httpError(errorStatusCode, msg) { }
		serverError( const char* msg ):
			httpError(500, msg) { }
		serverError( int statusCode ):
			httpError(statusCode) { }
};

#endif
