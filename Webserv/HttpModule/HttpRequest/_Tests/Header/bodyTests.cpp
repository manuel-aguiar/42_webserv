

#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void bodyTests(int &testNumber)
{
	Http::Request	Request;
	std::string		requestData;

	TEST_HEADER("Http Request - Body");

	TEST_HEADER("Passing Tests");

	////////////////////////////////////////////

	TEST_INTRO(testNumber++);

	requestData =
		std::string("POST /api/v1/users HTTP/1.1\r\n") +
		"Host: example.com\r\n" +
		"Authorization: Bearer YOUR_ACCESS_TOKEN\r\n" +
		"Content-Type: application/json\r\n" +
		"Accept: application/json\r\n" +
		"Content-Length: 78\r\n" +
		"User-Agent: MyApp/1.0\r\n\r\n"
		"{\"name\":\"John Doe\",\"email\":\"johndoe@example.com\",\"password\":\"securepassword\"}";

	try
	{
		EXPECT_EQUAL(Http::Request.parse(requestData), (int)Http::Status::OK, "Should pass");
		TEST_PASSED_MSG("Common request");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	////////////////////////////////////////////

	TEST_FOOTER;
}