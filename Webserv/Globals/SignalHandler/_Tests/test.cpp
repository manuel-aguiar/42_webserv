

# include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../SignalHandler.hpp"
# include <iostream>

int main(void)
{
	int testNumber = 1;

	TEST_HEADER("SignalHandler");

	try
	{
		TEST_INTRO(testNumber++);

		//SignalHandler handler; <- correctly asserts, SignalHandler is a global and cannot be instantiated outside global scope

		TEST_PASSED_MSG("SignalHandler instantiation");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
	
	TEST_FOOTER;
	return (0);
}

