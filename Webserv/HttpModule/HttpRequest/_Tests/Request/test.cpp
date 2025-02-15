#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void stateTransitionTests(int &testNumber)
{
	HttpRequest httpRequest;
	std::string requestData;

	TEST_HEADER("Http Request - State Machine Tests");

	TEST_HEADER("Initial State Tests");

	TEST_INTRO(testNumber++);
	try
	{
		EXPECT_EQUAL(httpRequest.isStarted(), false, "Should be in IDLE state initially");
		EXPECT_EQUAL(httpRequest.isError(), false, "Should not be in ERROR state initially");
		EXPECT_EQUAL(httpRequest.isIncomplete(), false, "Should not be in INCOMPLETE state initially");
		EXPECT_EQUAL(httpRequest.isCompleted(), false, "Should not be in COMPLETED state initially");
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