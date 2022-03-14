#include "config_parsing/Token.hpp"
#include "config_parsing/Lexer.hpp"
#include "gtest/gtest.h"

/* void	test_good_construction( Token::token_type type, std::string value ) */
/* { */
/* 	Token	tested_token(type, value); */

/* 	EXPECT_EQ(typeid(tested_token.getType()), typeid(type)); */
/* 	EXPECT_EQ(tested_token.getValue(), value); */
/* } */

TEST(TokenSuite, Constructors)
{
	Token	empty_constructor;
	EXPECT_EQ(typeid(empty_constructor.getType()), typeid(Token::end_of_file));

	Token	standard_constructor(Token::word, "bonjour");
	EXPECT_EQ(typeid(standard_constructor.getType()), typeid(Token::word));

	Token	copy_constructor(standard_constructor);
	EXPECT_EQ(typeid(copy_constructor.getType()), typeid(Token::word));
}

/* TEST(TokenSuite, failTest) */
/* { */
/* 	Token test; */
/* 	EXPECT_EQ(test.getType(), Token::path); */
/* } */

