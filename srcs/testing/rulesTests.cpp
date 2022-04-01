/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rulesTests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pohl <pohl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 11:54:28 by pohl              #+#    #+#             */
/*   Updated: 2022/04/01 15:52:39 by pohl             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configParsing/Parser.hpp"
#include "configParsing/Rules.hpp"
#include "gtest/gtest.h"

TEST(Rules, RulesTests)
{
	std::string 	pathReceivedFromRequest = "/index.html";
	Parser			parser("./config_files/everyRight.conf");
	ConfigFileNode	configFile = parser.getConfigFile();
	Rules			generalRules;

	// Need to have a function somewhere to select the right serverNode
	// we will use the last server for these tests
	generalRules.setValues(configFile.getServerList()[3], pathReceivedFromRequest.c_str());
	EXPECT_EQ(configFile.getServerList()[3].getServerRules().errorPage[403], generalRules.errorPage[403]);
	EXPECT_TRUE (generalRules.isCgi("/index.py"));
	EXPECT_FALSE(generalRules.isCgi("/index.html"));
	pathReceivedFromRequest = "/www/index.html";
	generalRules.setValues(configFile.getServerList()[3], pathReceivedFromRequest.c_str());
	EXPECT_EQ(configFile.getServerList()[3].getLocationList()[0].errorPage[403], generalRules.errorPage[403]);
	EXPECT_TRUE (generalRules.isCgi("/www/index.py"));
	EXPECT_FALSE(generalRules.isCgi("/www/index.html"));
	EXPECT_TRUE (generalRules.isCgi("/index.py"));
	EXPECT_TRUE (generalRules.isCgi("/index.py/extra/path/info.txt"));
	EXPECT_TRUE (generalRules.isCgi("/index.py/extra/path/info.txt?query=string"));
	EXPECT_TRUE (generalRules.isCgi("/index.py?query=string"));
	EXPECT_FALSE(generalRules.isCgi("/index.python?query=string"));
	EXPECT_FALSE(generalRules.isCgi("/index.python"));
	EXPECT_FALSE(generalRules.isCgi("/index.html"));
}
