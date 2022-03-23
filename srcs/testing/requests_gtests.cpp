#include "gtest/gtest.h"
#include "requests/parsing.hpp"
#include "requests/requests.hpp"
#include "requests/requestHeaderToken.hpp"

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
	const char * validTests[] = { "HTTP/1.0",  "HTTP/0.0", "HTTP/9.0", "HTTP/1.9",NULL };
	const char * invalidTests[] = { "HTTP/1.", "HTTP/.0", " HTT/1.0", "htt/1.1\b\n", "paafyaadix"
									" GET", "", "GET27", NULL };
	for (int i = 0; validTests[i]; i++)
		EXPECT_EQ( parseHttpVersion( validTests[i] ), validTests[i] );
	
	for (int i = 0; invalidTests[i]; i++)
		EXPECT_TRUE( parseHttpVersion( invalidTests[i ]) == NULL );
}

TEST(RequestSuite, createRequestTests)
{
	const char * validTests[] = {"GET www.doug.fr HTTP/1.0\b\n", "POST * HTTP/0.0\b\n", NULL};
	const char * invalidTests[] = {"hello this is the place", "hohoho", "bip boup", "",
							 "POT * HTTP/0.0\b", "GEt www.tamer.fr hTTP/1.0\b\n",
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

TEST(RequestSuite, createRequestTypeCheck)
{
	const char *getTests[] = { "GET www.Talleyrand.fr HTTP/1.0\b\n" , NULL };
	const char *postTests[] = { "POST www.Bonaparte.fr HTTP/1.0\b\n" , NULL };
	const char *deleteTests[] = { "DELETE www.Mauvaisappart.fr HTTP/1.0\b\n" , NULL };

	iRequest * result = NULL;
	
	for (int i = 0; getTests[i]; i++)
	{
		iRequest * check = NULL;		
		result = iRequest::createRequest(std::string(getTests[i]));
		check = dynamic_cast<getRequest *>(result);
		EXPECT_TRUE(check != NULL) << "fail for input : " << getTests[i];
	}

	for (int i = 0; postTests[i]; i++)
	{
		iRequest * check = NULL;		
		result = iRequest::createRequest(std::string(postTests[i]));
		check = dynamic_cast<postRequest *>(result);
		EXPECT_TRUE(check != NULL)<< "fail for input : " << postTests[i];
	}

	for (int i = 0; deleteTests[i]; i++)
	{
		iRequest * check = NULL;		
		result = iRequest::createRequest(std::string(deleteTests[i]));
		check = dynamic_cast<deleteRequest *>(result);
		EXPECT_TRUE(check != NULL) << "fail for input : " << deleteTests[i];
	}

}

TEST(requestHeaderTokenSuite, FirefoxGetRequestTest)
{
	const char * request_line = { "GET / HTTP/1.1\b\n" };
	const char * request_header = { "Host: localhost:8080\b\nConnection: keep-alive\b\nCache-Control: max-age=0\b\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\b\nsec-ch-ua-mobile: ?0\b\nsec-ch-ua-platform: \"Linux\"\b\nUpgrade-Insecure-Requests: 1\b\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\b\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.9\b\nSec-Fetch-Site: none\b\nSec-Fetch-Mode: navigate\b\nSec-Fetch-User: ?1\b\nSec-Fetch-Dest: document\b\nAccept-Encoding: gzip, deflate, br\b\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\b\n\b\n"};

	iRequest * result = iRequest::createRequest(request_line);

	std::vector<requestHeaderToken> vec = parseRequestHeader(request_header);

	iRequest * check = dynamic_cast<getRequest *>(result);
	EXPECT_TRUE(check != NULL);


	const char *small_request_header = {"Host: localhost:8080\b\nConnection: keep-alive\b\nCache-Control: max-age=0\b\n\b\n"};
	const char *expected_field[] = {"Host", "Connection", "Cache-Control", NULL};
	const char *expected_field_2[] = {"localhost:8080", "keep-alive", "max-age=0", NULL};
	std::vector<requestHeaderToken> vec1;
	try {
		vec1 = parseRequestHeader(small_request_header);
	}
	catch (std::exception & e) {
	}
	std::cout << vec1.size() << std::endl;
	EXPECT_TRUE(vec1.size() == 3);
	for (size_t i = 0; i < vec1.size(); i++)
	{
		EXPECT_TRUE((vec1[i]._token.first) == std::string(expected_field[i])) << "got " << vec1[i]._token.first << " instead of " << expected_field[i];

//		ASSERT_EQ(vec1[i]._input.size(), size_t(1));

		EXPECT_TRUE((vec1[i]._token.second) == std::string(expected_field_2[i]))  << "got " << vec1[i]._token.second << " instead of " << expected_field_2[i];
	}
	
}
