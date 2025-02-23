
// targets

# include "../../HttpModule/HttpModule.hpp"


// dependencies
# include "../../../ServerContext/ServerContext.hpp"

// helpers
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"


// C++ headers
#include <iostream>


int main(void)
{
	int testNumber = 1;
	TEST_HEADER("HttpModule Compile Everything");
	ServerContext context;

	try
	{
		TEST_INTRO(testNumber++);

		Http::Module module(10, context);


		TEST_PASSED_MSG("HttpModule constructor");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	TEST_FOOTER;
}