/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:11:42 by pohl              #+#    #+#             */
/*   Updated: 2022/03/24 16:53:44 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "configParsing/Parser.hpp"

int main( int argc, char **argv )
{
	(void)argc;
	Parser			parser("./srcs/testing/config_files/everyRight.conf");
	ConfigFileNode	result = parser.getConfigFile();
	result.latestServer().latestLocation().isCgi("/www/tst.php/coucou/text.html?asdf");
}
