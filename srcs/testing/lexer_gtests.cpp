#include "config_parsing/Token.hpp"
#include "config_parsing/Lexer.hpp"
#include "gtest/gtest.h"

TEST(TokenSuite, ctor)
{
	Token test;
	EXPECT_EQ(test.getType(), Token::end_of_file);
}

TEST(TokenSuite, failTest)
{
	Token test;
	EXPECT_EQ(test.getType(), Token::path);
}

