#include "configParsing/Token.hpp"
#include "configParsing/Lexer.hpp"
#include "configParsing/Exception.hpp"
#include "gtest/gtest.h"

void	testGoodConstruction( Token::tokenType type, std::string value )
{
	Token	testedToken(type, value);

	EXPECT_EQ(testedToken.getType(), type);
	EXPECT_EQ(testedToken.getValue(), value);
}

void	testSizeConversion( std::string sizeStr, size_t expectedSize )
{
	Token	sizeToken(Token::size, sizeStr);

	EXPECT_EQ(sizeToken.sizeAtoi(), expectedSize);
}

TEST(TokenSuite, Constructors)
{
	Token	emptyConstructor;
	EXPECT_EQ(emptyConstructor.getType(), Token::endOfFile);

	Token	standardConstructor(Token::word, "bonjour");
	EXPECT_EQ(standardConstructor.getType(), Token::word);

	Token	copyConstructor(standardConstructor);
	EXPECT_EQ(copyConstructor.getType(), Token::word);

	testGoodConstruction(Token::openingBracket, "");
	testGoodConstruction(Token::closingBracket, "");
	testGoodConstruction(Token::semicolon, "");
	testGoodConstruction(Token::path, "./bonjour/pouet/");
	testGoodConstruction(Token::ipAddress, "127.0.0.1");
	testGoodConstruction(Token::size, "123Mib");
	testGoodConstruction(Token::word, "server");
	testGoodConstruction(Token::number, "42");

	Token	otherToken(Token::number, "42");
	standardConstructor = otherToken;
	EXPECT_EQ(otherToken.getType(), standardConstructor.getType());
	EXPECT_EQ(otherToken.getValue(), standardConstructor.getValue());
	EXPECT_EQ(otherToken, standardConstructor);
}

TEST(TokenSuite, SizeConversion)
{
	testSizeConversion("12", 12);
	testSizeConversion("12K", 12288);
	testSizeConversion("12M", 12582912);
	testSizeConversion("12G", 12884901888);
	testSizeConversion("12T", 13194139533312);
}

void	testFileOpeningFailure( std::string filename )
{
	try {
		Lexer	standardConstructor(filename);
		ADD_FAILURE();
	}
	catch ( ParsingException& e ) {
		SUCCEED();
	}
	catch ( std::exception &e ) {
		ADD_FAILURE();
	}
}

TEST(LexerSuite, FileOpening)
{
	Lexer	emptyConstructor;

	testFileOpeningFailure("./Bonjour"); // doesn't exist
	/* testFileOpeningFailure("./config_files/unauthorized.conf"); // no read rights */
	Lexer	successfulOpening("./config_files/single_elements.conf");
}

TEST(LexerSuite, Reading)
{
	Lexer	emptyFile("./config_files/empty_file.conf");

	EXPECT_EQ(emptyFile.getNextToken().getType(), Token::endOfFile);
	// If we keep reading the file, it should not crash and keep returning eof
	EXPECT_EQ(emptyFile.getNextToken().getType(), Token::endOfFile);

	// Only valid elements
	Lexer	singleElements("./config_files/single_elements.conf");
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::openingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::closingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::semicolon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::colon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "."));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "/"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "./"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "/www/serv"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "/www/serv/"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "./www/serv"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "./www/serv/"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "./../././"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "./www/"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "/1/2/3/4"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, ".//nul"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "bonjour"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "command"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "server"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "path"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "a"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "142"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "5"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "100000000"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::ipAddress, "142.5.36.192"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::ipAddress, "127.0.0.1"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::ipAddress, "255.255.255.255"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::ipAddress, "555.555.555.555"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::ipAddress, "1245.12.12.12"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::ipAddress, "127.0.0.1"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::colon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "8000"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::colon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "8000"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "localhost"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::colon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "8000"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12K"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12M"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "12T"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::size, "42.12G"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::endOfFile, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::endOfFile, ""));

	singleElements.openFile("./config_files/tricky_spaces.conf");
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "server"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::openingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "listen"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "4000"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::semicolon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "error_page"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "404"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "./404.html"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::semicolon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::closingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "server"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::openingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "index"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "homepage.html"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "truc.csv"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "x.html"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::path, "/www/index.html"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::semicolon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "listen"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "8000"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::semicolon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::closingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "server"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::openingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "listen"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::number, "8080"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "localhost"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::semicolon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "server_name"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::word, "localhost"));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::semicolon, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::closingBracket, ""));
	EXPECT_EQ(singleElements.getNextToken(), Token(Token::endOfFile, ""));
}

