// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parsing.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 12:39:47 by pcharton          #+#    #+#             //
//   Updated: 2022/03/15 13:04:08 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/parsing.hpp"

const char *requestTable[] = { "GET", "POST", "DELETE", NULL };

const char *parseMethod(const char * input)
{
	for (int i = 0; requestTable[i]; i++)
		if (!strcmp(input, requestTable[i]))
			return (requestTable[i]);
	return (NULL);
}
