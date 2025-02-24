#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void lineTests(int &testNumber)
{
	ServerContext context;

	Http::Request Request(context);
	std::string	requestData;

	TEST_HEADER("Http Request - Line");

	TEST_HEADER("Passing Tests");

	////////////////////////////////////////////

	TEST_INTRO(testNumber++);

	requestData =
		std::string("GET /index.html HTTP/1.1\r\n") +
		"Host: www.example.com\r\n" +
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n" +
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
		"Accept-Encoding: gzip, deflate, br\r\n" +
		"Accept-Language: en-US,en;q=0.5\r\n" +
		"HttpConnection: keep-alive\r\n" +
		"\r\n";
	Buffer<2048> buffer;
	buffer.push(requestData.c_str(), requestData.size());
	try
	{
		Request.parse(buffer);

		const Http::RequestData& data = Request.getData();
		EXPECT_EQUAL(data.status, (int)Http::Status::OK, "Should pass");
		TEST_PASSED_MSG("Common GET request");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	////////////////////////////////////////////

	TEST_FOOTER;
}