#include "gtest/gtest.h"
#include "requests/parsing.hpp"

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
