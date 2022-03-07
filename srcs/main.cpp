/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 09:11:42 by pohl              #+#    #+#             */
/*   Updated: 2022/03/07 10:36:55 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#ifdef TEST
# include "testing.hpp"
#endif

int main( int argc, char **argv )
{
#ifdef TEST
	std::cout << "Debug mode ON" << std::endl;
	return test_main(argc, argv);
#endif
	std::cout << "Debug mode OFF" << std::endl;
	std::ifstream	config_file;

	if (argc != 2)
	{
		std::cerr << "usage: ./webserv [config file]" << std::endl;
		return 1;
	}
	config_file.open(argv[1]);
	if (!config_file.good())
	{
		std::cerr << "Problem with config file" << std::endl;
		return 1;
	}
}
