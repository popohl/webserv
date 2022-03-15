// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parsing.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 12:39:47 by pcharton          #+#    #+#             //
//   Updated: 2022/03/15 13:38:31 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "requests/parsing.hpp"

const char *methodTable[] = { "GET", "POST", "DELETE", NULL };

//these functions are not NULL safe

//RequestLine parsing

const char *parseMethod(const char * input)
{
	for (int i = 0; methodTable[i]; i++)
		if (!strcmp(input, methodTable[i]))
			return (methodTable[i]);
	return (NULL);
}

//expect array of the form "HTTP/1.1\b\n

const char *parseHttpVersion(const char *input)
{
	if (strlen(input) == 10)
	{
		if (!strncmp("HTTP/", &input[0], 5)
			&& isdigit(input[5])
			&& (input[6] == '.')
			&& isdigit(input[7])
			&& isCRLF(&input[8]))
			return (input);
	}
	return (NULL);
}
			
bool isCRLF(const char *input)
{
	std::cout << "paf" << std::endl;
	if (!strcmp("\b\n", input))
		return (true);
	else
		return (false);
}
