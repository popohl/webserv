#include "configParsing/Parser.hpp"
#include "configParsing/Exception.hpp"
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
	EXPECT_EQ(result.getServerList()[3].getLocationList().size(), (size_t)3);
	EXPECT_EQ(result.getServerList()[0].getServerRules().autoindex, false);
	EXPECT_EQ(result.getServerList()[0].getServerRules().listenPort, 8000);
	EXPECT_EQ(result.getServerList()[1].getServerRules().autoindex, false);
	EXPECT_EQ(result.getServerList()[1].getServerRules().clientMaxBodySize, 44040192);
	EXPECT_EQ(result.getServerList()[1].getServerRules().errorPage[404], "./404.html");
	EXPECT_EQ(result.getServerList()[1].getServerRules().listenAddress, "127.0.0.1");
	EXPECT_EQ(result.getServerList()[1].getServerRules().listenPort, 7000);
	EXPECT_EQ(result.getServerList()[1].getServerRules().serverName[0], "www.example.fr");
	EXPECT_EQ(result.getServerList()[1].getServerRules().serverName.size(), (size_t)3);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[0].allowedMethod, 0b0111);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].allowedMethod, 0b0101);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].redirectCode, 302);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].redirectUri, "www.archlinux.org");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].root, "/www/rootsite/");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].index.size(), (size_t)1);
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].index[0], "homepage.html");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].uploadPath, "/www/uploads");
	EXPECT_EQ(result.getServerList()[3].getServerRules().errorPage[401], "./server401.html");
	EXPECT_EQ(result.getServerList()[3].getServerRules().errorPage[403], "./server403.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].errorPage[401], "./server401.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].errorPage[403], "./location403.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].errorPage[412], "./location412.html");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].cgiExtension, "php");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].cgiPath, "/bin/php-cgi");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[0].cgiPath, "");
	EXPECT_EQ(result.getServerList()[3].getServerRules().cgiExtension, "py");
	EXPECT_EQ(result.getServerList()[3].getServerRules().cgiPath, "/cgi-bin/python");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].cgiExtension, "py");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].cgiPath, "/cgi-bin/python");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[0].locationPath, "/");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].locationPath, "/www/");

	EXPECT_EQ(result.getServerList()[2].getLocationList()[0].getPathFromLocation("/www/bonjour/etc.html"),
			"/www/bonjour/etc.html");
	EXPECT_EQ(result.getServerList()[2].getLocationList()[1].getPathFromLocation("/www/bonjour/etc.html"),
			"/www/rootsite/bonjour/etc.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[0].getPathFromLocation("/www/downloads/bonjour/etc.html"),
			"/www/downloads/bonjour/etc.html");
	EXPECT_EQ(result.getServerList()[3].getLocationList()[1].getPathFromLocation("/www/downloads/bonjour/etc.html"),
			"/etc/systemd/bonjour/etc.html");

	const LocationRules* selectedLocation = result.getServerList()[2].getLocationFromUrl("/web/pouet.html");
	EXPECT_EQ(selectedLocation, &result.getServerList()[2].getLocationList()[0]);
	selectedLocation = result.getServerList()[2].getLocationFromUrl("/www/pouet.html");
	EXPECT_EQ(selectedLocation, &result.getServerList()[2].getLocationList()[1]);
	selectedLocation = result.getServerList()[3].getLocationFromUrl("/www/pouet.html");
	EXPECT_EQ(selectedLocation, &result.getServerList()[3].getLocationList()[0]);
	selectedLocation = result.getServerList()[3].getLocationFromUrl("/www/downloads/pouet.html");
	EXPECT_EQ(selectedLocation, &result.getServerList()[3].getLocationList()[1]);
	selectedLocation = result.getServerList()[3].getLocationFromUrl("/dont_exist/nope/");
	EXPECT_EQ(selectedLocation, (const LocationRules*)NULL);

	std::set<int>	listeningPorts = result.getListeningPorts();
	std::set<int>::iterator it = listeningPorts.begin();
	EXPECT_EQ(*(it++), 7000);
	EXPECT_EQ(*(it++), 8000);
	EXPECT_EQ(*(it++), 9000);
	EXPECT_EQ(it, listeningPorts.end());
}

bool checkErrorParsing( std::string fileName )
{
	try {
		Parser parser(fileName);
		return false;
	}
	catch ( ParsingException& e ) {
		return true;
	}
	catch ( std::exception &e ) {
		return false;
	}
}

TEST(ParserSuite, ErrorParsing)
{
	EXPECT_TRUE(checkErrorParsing("./config_files/error_listen1.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_listen2.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_listen3.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_listen4.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_listen5.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_listen6.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_allowed_method1.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_allowed_method2.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_autoindex1.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_autoindex2.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_cgi_extension1.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_cgi_extension2.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_cgi_extension3.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_cgi_path.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_client_max_body_size.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_forgot_semicolon.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_index.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_redirect.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_root.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_location_upload_path.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_autoindex1.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_autoindex2.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_client_max_body_size.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_empty_rule.conf"));
	/* EXPECT_TRUE(checkErrorParsing("./config_files/error_server_error_page.conf")); Error codes checks must be implemented before adding this test */
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_index1.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_index2.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_name1.conf"));
	EXPECT_TRUE(checkErrorParsing("./config_files/error_server_name2.conf"));
}
