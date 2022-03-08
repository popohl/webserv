/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 10:30:46 by pohl              #+#    #+#             */
/*   Updated: 2022/03/08 11:21:04 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.hpp"

int		test_main( int argc, char **argv )
{
	(void)argc;(void)argv;
	/* if (argc != 2) */
	/* 	throw std::invalid_argument("usage: ./webserv [config file]"); */
	/* token_tests(); */
	lexer_tests();
	return 0;
}
