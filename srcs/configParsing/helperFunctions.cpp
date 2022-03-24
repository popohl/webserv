/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperFunctions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 12:03:03 by pohl              #+#    #+#             */
/*   Updated: 2022/03/23 12:15:08 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

bool	hasCgiExtension( std::string uri, std::string cgiExtension )
{
	if (cgiExtension == "")
		return false;
	uri.erase(0, uri.find_first_of('.') + 1);
	if (uri.rfind(cgiExtension, 0) == 0)
		return true;
	return false;
}
