#include "gtest/gtest.h"
#include "requests/requests.hpp"
#include "requests/requestBase.hpp"
/*
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
	const char * invalidTests[] = { "HTTP/1.", "HTTP/.0", " HTT/1.0", "htt/1.1\r\n", "paafyaadix"
									" GET", "", "GET27", NULL };
	for (int i = 0; validTests[i]; i++)
		EXPECT_EQ( parseHttpVersion( validTests[i] ), validTests[i] );
	
	for (int i = 0; invalidTests[i]; i++)
		EXPECT_TRUE( parseHttpVersion( invalidTests[i ]) == NULL );
}
*/

TEST(RequestSuite, createRequestTests)
{
	const char * validTests[] = {"GET www.doug.fr HTTP/1.0\r\n", "POST * HTTP/0.0\r\n", NULL};
	const char * invalidTests[] = {"hello this is the place", "hohoho", "bip boup", "",
							 "POT * HTTP/0.0\r", "GEt www.tamer.fr hTTP/1.0\r\n",
							 "HEAD * HTTP/1.1\r\n", NULL};
	for (int i = 0; validTests[i]; i++)
	{
		std::string tmp(validTests[i]);
		iRequest * result = iRequest::createRequest(tmp);
		EXPECT_TRUE( result != NULL) << "fail for input : " << validTests[i];
		delete result;
	}
	for (int i = 0; invalidTests[i]; i++)
	{
		std::string tmp(invalidTests[i]);
		iRequest * result = iRequest::createRequest(tmp);
		EXPECT_TRUE( result == NULL) << "fail for input : " << invalidTests[i];
		delete result;
	}
}

TEST(RequestSuite, createRequestTypeCheck)
{
	const char *getTests[] = { "GET www.Talleyrand.fr HTTP/1.0\r\n" , NULL };
	const char *postTests[] = { "POST www.Bonaparte.fr HTTP/1.0\r\n" , NULL };
	const char *deleteTests[] = { "DELETE www.Mauvaisappart.fr HTTP/1.0\r\n" , NULL };

	iRequest * result = NULL;
	
	for (int i = 0; getTests[i]; i++)
	{
		iRequest * check = NULL;		
		std::string tmp(getTests[i]);
		result = iRequest::createRequest(tmp);
		check = dynamic_cast<getRequest *>(result);
		EXPECT_TRUE(check != NULL) << "fail for input : " << getTests[i];
	}

	for (int i = 0; postTests[i]; i++)
	{
		iRequest * check = NULL;
		std::string tmp(postTests[i]);
		result = iRequest::createRequest(tmp);
		check = dynamic_cast<postRequest *>(result);
		EXPECT_TRUE(check != NULL)<< "fail for input : " << postTests[i];
	}

	for (int i = 0; deleteTests[i]; i++)
	{
		iRequest * check = NULL;
		std::string tmp(deleteTests[i]);
		result = iRequest::createRequest(tmp);
		check = dynamic_cast<deleteRequest *>(result);
		EXPECT_TRUE(check != NULL) << "fail for input : " << deleteTests[i];
	}

}

/*
TEST(requestHeaderTokenSuite, FirefoxGetRequestTest)
{
	const char * request_line = { "GET / HTTP/1.1\r\n" };
	const char * request_header = { "Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n"};

	iRequest * result = iRequest::createRequest(request_line);

	std::vector<requestHeaderToken> vec = parseRequestHeader(request_header);

	iRequest * check = dynamic_cast<getRequest *>(result);
	EXPECT_TRUE(check != NULL);


	const char *small_request_header = {"Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n"};
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
*/


TEST(requestHeaderSuite, FirefoxGetRequestTestv2)
{
	const char * request_line = { "GET / HTTP/1.1\r\n" };
	const char * request_header = { "Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n"};

	std::string input(request_line);
	iRequest * result = iRequest::createRequest(input);

	requestBase test;
	std::string header(request_header);
	test.parseHeader(header);

	iRequest * check = dynamic_cast<getRequest *>(result);
	EXPECT_TRUE(check != NULL);


	const char *small_request_header = {"Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n"};
	const char *expected_field[] = {"Host", "Connection", "Cache-Control", NULL};
	const char *expected_field_2[] = {"localhost:8080", "keep-alive", "max-age=0", NULL};
	requestBase test1;


	try {
		test1.parseHeader(small_request_header);
	}
	catch (std::exception & e) {
	}

	EXPECT_TRUE(test1._header.size() == 3);
	for (size_t i = 0; expected_field[i]; i++)
	{

		std::map<std::string, std::string>::iterator it = test1._header.find(expected_field[i]);
		
		EXPECT_TRUE(it->first == std::string(expected_field[i])) << "got " << it->first << " instead of " << expected_field[i];
		EXPECT_TRUE(it->second == std::string(expected_field_2[i]))  << "got " << it->second << " instead of " << expected_field_2[i];
	}
	
}

TEST(requestSuite, TestWithCompleteHeader)
{
	std::string input ("GET / HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n");

	iRequest * result = iRequest::createRequest(input);
	EXPECT_TRUE(result != NULL) << "request did not allocate getRequest";
	getRequest * cast_result = dynamic_cast<getRequest *>(result);
	EXPECT_TRUE(cast_result != NULL) << "allocated iRequest is not a getRequest *";
	EXPECT_STREQ(result->getRequestURI().c_str(), "/") << "for request URI, expected / and got " << result->getRequestURI();
}

TEST(requestSuite, TestWithHeaderInTwoParts)
{
	std::string input ("GET / HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\n");
	std::string input2 ("User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n");

	iRequest * result = iRequest::createRequest(input);
	EXPECT_TRUE(result != NULL) << "request did not allocate getRequest";
	getRequest * cast_result = dynamic_cast<getRequest *>(result);
	EXPECT_TRUE(cast_result != NULL) << "allocated iRequest is not a getRequest *";
	EXPECT_STREQ(result->getRequestURI().c_str(), "/") << "for request URI, expected / and got " << result->getRequestURI();
	//state of the request before sending the second input
	EXPECT_FALSE(result->receivingisDone());
	result->_message.parseRequest(input2);
	EXPECT_TRUE(result->receivingisDone());
}


TEST(requestHeaderSuite, InvalidHeaderRequestTests)
{
	const char *invalidTests[] = { "localtruc yoyoyo",
								   "Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nCacolac\r\n",
								   "Bipip\r\nKrack",
								   NULL};

	for (int i = 0; invalidTests[i]; i++)
	{
		EXPECT_THROW({
				try
				{
					requestBase test;
					test.parseHeader(std::string(invalidTests[i]));
				}
				catch (const unfinishedHeader & e) {
					EXPECT_STREQ("Header is not correctly terminated", e.what());
					throw;
				}
			}, unfinishedHeader);
	}
}

TEST(requestHeaderSuite, InvalidSyntaxRequestTests)
{
	const char *invalidTests[] = { "localtruc: yoyoyo\r\nCACAhoho",
								   "Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nCacolac\r\n",
								   NULL};

	for (int i = 0; invalidTests[i]; i++)
	{
		EXPECT_THROW({
				try
				{
					requestBase test;
					test.parseHeader(std::string(invalidTests[i]));
				}
				catch (const malformedHeader & e) {
					EXPECT_STREQ("Header has syntax error", e.what());
					throw;
				}
			}, unfinishedHeader);
	}
}

TEST(RequestBodySuite, simpleTest)
{
	const char * input = { "Host: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\nContent-Length:83\r\n\r\nCeci est un exemple de fichier qui pourrait etre envoye dans le corps de la requete"};
	const char *body = {"Ceci est un exemple de fichier qui pourrait etre envoye dans le corps de la requete"};

	requestBase test;
	test.parseRequest(std::string(input));
	EXPECT_STREQ(test._body.c_str(), body);

	test.parseRequest(std::string(input));
	EXPECT_STREQ(test._body.c_str(), body);
}
