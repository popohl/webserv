#include "config_parsing/Parser.hpp"
#include "gtest/gtest.h"

TEST(ParserSuite, EmptyFile)
{
	Parser	parser("./config_files/empty_file.conf");
	ConfigFileNode	result = parser.getConfigFile();

	EXPECT_EQ(result.getServerList().size(), (size_t)0);
}

TEST(ParserSuite, EveryRightPossibilityFile)
{
	Parser	parser("./config_files/everyRight.conf");
	ConfigFileNode	result = parser.getConfigFile();

	EXPECT_EQ(result.getServerList().size(), (size_t)4);
	EXPECT_EQ(result.getServerList()[0].getLocationList().size(), (size_t)0);
	EXPECT_EQ(result.getServerList()[1].getLocationList().size(), (size_t)0);
	EXPECT_EQ(result.getServerList()[2].getLocationList().size(), (size_t)2);
	EXPECT_EQ(result.getServerList()[3].getLocationList().size(), (size_t)1);
	EXPECT_EQ(result.getServerList()[0].getServerRules().autoindex, false);
	EXPECT_EQ(result.getServerList()[0].getServerRules().listen_port, 8000);
	EXPECT_EQ(result.getServerList()[1].getServerRules().autoindex, false);
	EXPECT_EQ(result.getServerList()[1].getServerRules().client_max_body_size, 44040192);
	EXPECT_EQ(result.getServerList()[1].getServerRules().error_page[404], "./404.html");
	EXPECT_EQ(result.getServerList()[1].getServerRules().listen_address, "127.0.0.1");
	EXPECT_EQ(result.getServerList()[1].getServerRules().listen_port, 8000);
	EXPECT_EQ(result.getServerList()[1].getServerRules().server_name[0], "www.example.fr");
	EXPECT_EQ(result.getServerList()[1].getServerRules().server_name.size(), (size_t)3);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].redirect_code, 302);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].redirect_uri, "www.archlinux.org");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].root, "/www/rootsite/");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].index.size(), (size_t)1);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].index[0], "homepage.html");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].upload_path, "/www/uploads");
	EXPECT_EQ(result.getServerList()[3].getServerRules().error_page[401], "./server401.html");
	EXPECT_EQ(result.getServerList()[3].getServerRules().error_page[403], "./server403.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].error_page[401], "./server401.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].error_page[403], "./location403.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].error_page[412], "./location412.html");

}
