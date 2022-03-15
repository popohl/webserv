#include "gtest/gtest.h"
#include "requests/parsing.hpp"
#include "requests/requests.hpp"

TEST(RequestSuite, parseMethodTests)
{
	const char * validTests[] = { "GET", "POST", "DELETE", NULL };
	const char * invalidTests[] = { "get", "post", "delete", "coucou",
									" GET", "GET ", "GET27", NULL };
	for (int i = 0; validTests[i]; i++)
		EXPECT_EQ( parseMethod( validTests[i] ), validTests[i] );
	
	for (int i = 0; invalidTests[i]; i++)
		EXPECT_TRUE( parseMethod( invalidTests[i ]) == NULL );
}

TEST(RequestSuite, parseHttpVersionTests)
{
	const char * validTests[] = { "HTTP/1.0\b\n",  "HTTP/0.0\b\n", "HTTP/9.0\b\n", "HTTP/1.9\b\n",NULL };
	const char * invalidTests[] = { "HTTP/1.0", "HTTP/1.0\n", " HTTP/1.0\b\n", "http/1.1\b\n", "paafyaadix"
									" GET", "", "GET27", NULL };
	for (int i = 0; validTests[i]; i++)
		EXPECT_EQ( parseHttpVersion( validTests[i] ), validTests[i] );
	
	for (int i = 0; invalidTests[i]; i++)
		EXPECT_TRUE( parseHttpVersion( invalidTests[i ]) == NULL );
}

TEST(RequestSuite, createRequestTests)
{
	const char * validTests[] = {"GET www.doug.fr HTTP/1.0\b\n", "POST * HTTP/0.0\b\n", NULL};
	char * invalidTests[] = {"hello this is the place", "hohoho", "bip boup", "",
							 "POST * HTTP/0.0\b", "GEt www.tamer.fr HTTP/1.0\b\n",
							 "HEAD * HTTP/1.1\b\n", NULL};
	for (int i = 0; validTests[i]; i++)
	{
		iRequest * result = iRequest::createRequest(std::string(validTests[i]));
		EXPECT_TRUE( result != NULL) << "fail for input : " << validTests[i];
		delete result;
	}
	for (int i = 0; invalidTests[i]; i++)
	{
		iRequest * result = iRequest::createRequest(std::string(invalidTests[i]));
		EXPECT_TRUE( result == NULL) << "fail for input : " << invalidTests[i];
		delete result;
	}
}
