
// targets

# include "../../HttpModule/HttpModule.hpp"


// dependencies
# include "../../../Globals/Globals.hpp"

// helpers
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"


// C++ headers
#include <iostream>


int main(void)
{
	int testNumber = 1;
	TEST_HEADER("HttpModule Compile Everything");
	Globals globals(NULL, NULL, NULL, NULL);

	try
	{
		TEST_INTRO(testNumber++);

		Http::Module module(10, globals);


		TEST_PASSED_MSG("HttpModule constructor");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	TEST_FOOTER;
}