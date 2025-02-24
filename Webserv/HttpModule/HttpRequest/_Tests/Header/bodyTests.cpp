

#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void bodyTests(int &testNumber)
{
	ServerContext	context;
	Buffer<2048>	buffer;
	
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
	buffer.clear();
	buffer.push(requestData.c_str(), requestData.size());
	try
	{
		Http::Request	Request(context);
		Request.parse(buffer);
		const Http::RequestData& data = Request.getData();

		EXPECT_EQUAL(data.status, (int)Http::Status::OK, "Should pass");
		TEST_PASSED_MSG("Common request");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	////////////////////////////////////////////

	TEST_FOOTER;
}