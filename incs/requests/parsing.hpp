// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parsing.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: pcharton <pcharton@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2022/03/15 12:23:52 by pcharton          #+#    #+#             //
//   Updated: 2022/03/28 11:13:04 by pcharton         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARSING_HPP
# define PARSING_HPP
/*
  Basic Rules : https://datatracker.ietf.org/doc/html/rfc2616#section-2.2

       OCTET          = <any 8-bit sequence of data>
       CHAR           = <any US-ASCII character (octets 0 - 127)>
       UPALPHA        = <any US-ASCII uppercase letter "A".."Z">
       LOALPHA        = <any US-ASCII lowercase letter "a".."z">
       ALPHA          = UPALPHA | LOALPHA
       DIGIT          = <any US-ASCII digit "0".."9">
       CTL            = <any US-ASCII control character
                        (octets 0 - 31) and DEL (127)>
       CR             = <US-ASCII CR, carriage return (13)>
       LF             = <US-ASCII LF, linefeed (10)>
       SP             = <US-ASCII SP, space (32)>
       HT             = <US-ASCII HT, horizontal-tab (9)>
       <">            = <US-ASCII double-quote mark (34)>
*/

/*

 */

# include <cstddef>
# include <string>
# include <cstring>

void parseRequestLine(std::string); //Input must be a line finished with \b\n

bool	checkMethod(const std::string &);
const char *parseRequestURI(const char *);
bool	checkHttpVersion(const std::string &);

bool isCRLF(const char *input);



#endif
