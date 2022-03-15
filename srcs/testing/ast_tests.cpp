#include "config_parsing/AST/ConfigFileNode.hpp"
#include "gtest/gtest.h"

TEST(ASTSuite, Constructors)
{
	ConfigFileNode	configFile;
	ServerNode		serverNode;
	LocationRules	locationRules;
	ServerRules		serverRules;

	serverRules.set_autoindex(!locationRules.get_autoindex());
	locationRules = LocationRules(serverRules);

	EXPECT_EQ(serverRules.get_autoindex(), locationRules.get_autoindex());

	configFile.addServer(serverNode);
	EXPECT_EQ(configFile.getServerList().size(), (size_t)1);
}
