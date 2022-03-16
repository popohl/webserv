#include "config_parsing/Token.hpp"
#include "config_parsing/Lexer.hpp"
#include "gtest/gtest.h"

void	test_good_construction( Token::token_type type, std::string value )
{
	Token	tested_token(type, value);

	EXPECT_EQ(tested_token.getType(), type);
	EXPECT_EQ(tested_token.getValue(), value);
}

void	test_size_conversion( std::string size_str, size_t expected_size )
{
	Token	size_token(Token::size, size_str);

	EXPECT_EQ(size_token.size_atoi(), expected_size);
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

	Token	other_token(Token::number, "42");
	standard_constructor = other_token;
	EXPECT_EQ(other_token.getType(), standard_constructor.getType());
	EXPECT_EQ(other_token.getValue(), standard_constructor.getValue());
	EXPECT_EQ(other_token, standard_constructor);
}

TEST(TokenSuite, SizeConversion)
{
	test_size_conversion("12", 12);
	test_size_conversion("12K", 12288);
	test_size_conversion("12M", 12582912);
	test_size_conversion("12G", 12884901888);
	test_size_conversion("12T", 13194139533312);
}

void	test_file_opening_failure( std::string filename )
{
	try {
		Lexer	standard_constructor(filename);
		ADD_FAILURE();
	}
	catch ( Lexer::file_exception &e ) {
		SUCCEED();
	}
	catch ( std::exception &e ) {
		ADD_FAILURE();
	}
}

TEST(LexerSuite, FileOpening)
{
	Lexer	empty_constructor;

	test_file_opening_failure("./Bonjour"); // doesn't exist
	/* test_file_opening_failure("./config_files/unauthorized.conf"); // no read rights */
	Lexer	successful_opening("./config_files/single_elements.conf");
}

TEST(LexerSuite, Reading)
{
	Lexer	empty_file("./config_files/empty_file.conf");

	EXPECT_EQ(empty_file.get_next_token().getType(), Token::end_of_file);
	// If we keep reading the file, it should not crash and keep returning eof
	EXPECT_EQ(empty_file.get_next_token().getType(), Token::end_of_file);

	// Only valid elements
	Lexer	single_elements("./config_files/single_elements.conf");
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::opening_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::closing_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::semicolon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::colon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "."));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "/"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "./"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "/www/serv"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "/www/serv/"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "./www/serv"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "./www/serv/"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "./../././"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "./www/"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "/1/2/3/4"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, ".//nul"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "bonjour"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "command"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "server"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "path"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "a"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "142"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "5"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "100000000"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::ip_address, "142.5.36.192"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::ip_address, "127.0.0.1"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::ip_address, "255.255.255.255"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::ip_address, "555.555.555.555"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::ip_address, "1245.12.12.12"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::ip_address, "127.0.0.1"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::colon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "8000"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::colon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "8000"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "localhost"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::colon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "8000"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12K"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12M"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "12T"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::size, "42.12G"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::end_of_file, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::end_of_file, ""));

	single_elements.open_file("./config_files/tricky_spaces.conf");
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "server"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::opening_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "listen"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "4000"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::semicolon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "error_page"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "404"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "./404.html"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::semicolon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::closing_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "server"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::opening_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "index"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "homepage.html"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "truc.csv"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "x.html"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::path, "/www/index.html"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::semicolon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "listen"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "8000"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::semicolon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::closing_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "server"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::opening_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "listen"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::number, "8080"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "localhost"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::semicolon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "server_name"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::word, "localhost"));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::semicolon, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::closing_bracket, ""));
	EXPECT_EQ(single_elements.get_next_token(), Token(Token::end_of_file, ""));
}
