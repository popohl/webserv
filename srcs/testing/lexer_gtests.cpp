#include "config_parsing/Token.hpp"
#include "config_parsing/Lexer.hpp"
#include "gtest/gtest.h"

void	test_good_construction( Token::token_type type, std::string value )
{
	Token	tested_token(type, value);

	EXPECT_EQ(tested_token.getType(), type);
	EXPECT_EQ(tested_token.getValue(), value);
}

TEST(TokenSuite, Constructors)
{
	Token	empty_constructor;
	EXPECT_EQ(empty_constructor.getType(), Token::end_of_file);

	Token	standard_constructor(Token::word, "bonjour");
	EXPECT_EQ(standard_constructor.getType(), Token::word);

	Token	copy_constructor(standard_constructor);
	EXPECT_EQ(copy_constructor.getType(), Token::word);

	test_good_construction(Token::opening_bracket, "");
	test_good_construction(Token::closing_bracket, "");
	test_good_construction(Token::semicolon, "");
	test_good_construction(Token::path, "./bonjour/pouet/");
	test_good_construction(Token::ip_address, "127.0.0.1");
	test_good_construction(Token::size, "123Mib");
	test_good_construction(Token::word, "server");
	test_good_construction(Token::number, "42");
}

/* TEST(TokenSuite, failTest) */
/* { */
/* 	Token test; */
/* 	EXPECT_EQ(test.getType(), Token::path); */
/* } */

