

# include <cstddef>
# include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../HttpDefinitions.hpp"
# include "../../../../GenericUtils/BufferView/BufferView.hpp"
extern const char* getStatusMessage(int statusCode);

int main()
{
	int testNumber = 1;
	TEST_HEADER("Http Error Codes Mapping");

	try
	{
		TEST_INTRO(testNumber++);

		// test before the array

		//			getStatusMessage(199); <- correctly asserts, not found in map

		// test beginning of the array
		EXPECT_EQUAL(BufferView(getStatusMessage(Http::Status::OK)), BufferView("OK"), "200 status code");
		
		// 			getStatusMessage(207); <- correctly asserts, not found in map
		
		// test middle of the array
		EXPECT_EQUAL(BufferView(getStatusMessage(Http::Status::NO_CONTENT)), BufferView("No Content"), "204 status code");

		// test end of the array
		EXPECT_EQUAL(BufferView(getStatusMessage(Http::Status::NETWORK_AUTHENTICATION_REQUIRED)), BufferView("Network Authentication Required"), "511 status code");	

		// test past the end of the array

		//			getStatusMessage(512); <- correctly asserts, not found in map

		TEST_PASSED_MSG("All tests passed");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
	

	TEST_FOOTER;
	return 0;
}
