#include "config_parsing/AST/ConfigFileNode.hpp"
#include "gtest/gtest.h"

TEST(ASTSuite, Constructors)
{
	ConfigFileNode	configFile;
	ServerNode		serverNode;
	LocationRules	locationRules;
	ServerRules		serverRules;

	serverRules.autoindex = !locationRules.autoindex;
	locationRules = LocationRules(serverRules);

	EXPECT_EQ(serverRules.autoindex, locationRules.autoindex);

	configFile.createNewServerNode();
	EXPECT_EQ(configFile.getServerList().size(), (size_t)1);
}
