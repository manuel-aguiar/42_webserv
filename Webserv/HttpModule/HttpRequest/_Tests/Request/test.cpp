#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void stateTransitionTests(int &testNumber)
{
	Http::Request Request;
	std::string requestData;

	TEST_HEADER("Http Request - State Machine Tests");

	TEST_HEADER("Initial State Tests");

	TEST_INTRO(testNumber++);
	try
	{
		EXPECT_EQUAL(Request.isStarted(), false, "Should be in IDLE state initially");
		EXPECT_EQUAL(Request.isError(), false, "Should not be in ERROR state initially");
		EXPECT_EQUAL(Request.isIncomplete(), false, "Should not be in INCOMPLETE state initially");
		EXPECT_EQUAL(Request.isCompleted(), false, "Should not be in COMPLETED state initially");
		TEST_PASSED_MSG("Initial state checks");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	TEST_FOOTER;
}

int main()
{
	int testNumber = 1;

	std::cout << "Starting HTTP Request Body Parser Tests...\n\n";

	stateTransitionTests(testNumber);

	std::cout << "\nAll tests completed!\n";
	return 0;
}