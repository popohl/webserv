// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestHeaderToken.hpp                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:49:25 by pcharton          #+#    #+#             //
//   Updated: 2022/03/17 18:06:10 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <string>
#include <vector>

struct requestHeaderToken {

	requestHeaderToken();
	requestHeaderToken(std::string type);
	

	std::string _type;
	std::vector<std::string> _input;
};

requestHeaderToken parseHost(std::string);
requestHeaderToken treatLine(const char *line);
std::vector<requestHeaderToken> parseRequestHeader(const char *input);

//requestHeaderToken::field findField(const char *);
