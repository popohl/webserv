// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   requestHeaderToken.hpp                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/17 16:49:25 by pcharton          #+#    #+#             //
//   Updated: 2022/03/23 20:58:27 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <string>
#include <vector>
#include <map>

struct requestHeaderToken {

	requestHeaderToken();
	requestHeaderToken(const std::pair <std::string, std::string> & rhs);

	std::pair<std::string, std::string> _token;
};

requestHeaderToken parseHost(std::string);
std::vector<requestHeaderToken> parseRequestHeader(const char *input);

std::vector<std::string>split_header_to_lines(const char *input);
requestHeaderToken treatLine(std::string line);

struct requestHeader {

	std::map<std::string, std::string> _header;
};


//requestHeaderToken::field findField(const char *);
