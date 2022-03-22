/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:34:53 by pohl              #+#    #+#             */
/*   Updated: 2022/03/18 14:36:05 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <stdexcept>

class ParsingException : public std::runtime_error {
	public:
		ParsingException(const char* msg) : std::runtime_error(msg) { }
};

#endif
