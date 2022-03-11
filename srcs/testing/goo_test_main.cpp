#include "gtest/gtest.h"
#include "parsing/Token.hpp"
#include "parsing/Lexer.hpp"

TEST(TokenSuite, ctor)
{
	Token test;
	EXPECT_EQ(test.getType(), Token::error);
}

TEST(TokenSuite, failTest)
{
	Token test;
	EXPECT_EQ(test.getType(), Token::cbracket);
}


int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    int returnValue;

    //Do whatever setup here you will need for your tests here
    //
    //

    returnValue =  RUN_ALL_TESTS();

    //Do Your teardown here if required
    //
    //

    return returnValue;
}
