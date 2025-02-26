#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void headerTests(int &testNumber)
{
	TEST_HEADER("Http Request - Header");

	TEST_HEADER("Passing Tests");

	ServerContext context;
	Buffer<2048> buffer;

	{
		TEST_INTRO(testNumber++);
		Http::Request Request(context);
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n" +
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
			"Accept-Encoding: gzip, deflate, br\r\n" +
			"Accept-Language: en-US,en;q=0.5\r\n" +
			"HttpConnection: keep-alive\r\n" +
			"\r\n";
		buffer.clear();
		buffer.push(requestData.c_str(), requestData.size());
		try
		{
			Request.parse(buffer);
			const Http::RequestData& data = Request.getData();
			EXPECT_EQUAL(data.status, (int)Http::Status::OK, "Should pass");
			TEST_PASSED_MSG("Common request");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	// Commenting this test, i think this is Response Duty

	//TEST_HEADER("Failing Tests");
	//
	//{
	//	TEST_INTRO(testNumber++);
	//	Http::Request Request(context);
	//	std::string requestData =
	//		std::string("GET /index.html HTTP/1.1\r\n") +
	//		"\r\n";
	//	buffer.clear();
	//	buffer.push(requestData.c_str(), requestData.size());
	//	try
	//	{
	//		Request.parse(buffer);
	//		const Http::RequestData& data = Request.getData();
	//		EXPECT_EQUAL(data.status, (int)Http::Status::BAD_REQUEST, "Should fail");
	//		TEST_PASSED_MSG("No headers");
	//	}
	//	catch(const std::exception& e)
	//	{
	//		TEST_FAILED_MSG(e.what());
	//	}
	//}

	{
		TEST_INTRO(testNumber++);
		Http::Request Request(context);
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			":\r\n" +
			"\r\n";
			buffer.clear();
			buffer.push(requestData.c_str(), requestData.size());
		try
		{
			Request.parse(buffer);
			const Http::RequestData& data = Request.getData();
			EXPECT_EQUAL(data.status, (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with no key nor value");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		Http::Request Request(context);
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			": Great value\r\n" +
			"\r\n";
		buffer.clear();
		buffer.push(requestData.c_str(), requestData.size());
		try
		{
			Request.parse(buffer);
				const Http::RequestData& data = Request.getData();
			EXPECT_EQUAL(data.status, (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with no key");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		Http::Request Request(context);
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"    : Great value\r\n" +
			"\r\n";
		buffer.clear();
		buffer.push(requestData.c_str(), requestData.size());
		try
		{
			Request.parse(buffer);
			const Http::RequestData& data = Request.getData();
			EXPECT_EQUAL(data.status, (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with space as key");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		Http::Request Request(context);
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"Great-Key:\r\n" +
			"\r\n";
		buffer.clear();
		buffer.push(requestData.c_str(), requestData.size());
		try
		{
			Request.parse(buffer);
			const Http::RequestData& data = Request.getData();
			EXPECT_EQUAL(data.status, (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with no value");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		Http::Request Request(context);
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"Great-Key:    \r\n" +
			"\r\n";
		buffer.clear();
		buffer.push(requestData.c_str(), requestData.size());
		try
		{
			Request.parse(buffer);
			const Http::RequestData& data = Request.getData();
			EXPECT_EQUAL(data.status, (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with space as value");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		Http::Request Request(context);
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n";
		buffer.clear();
		buffer.push(requestData.c_str(), requestData.size());
		try
		{
			Request.parse(buffer);
			const Http::RequestData& data = Request.getData();
			EXPECT_EQUAL(data.status, (int)Http::Status::OK, "no problems yet");
			EXPECT_EQUAL(Request.getParsingState(), (int)Http::Request::HEADERS, "Should be in HEADERS state, no double CRLF");
			TEST_PASSED_MSG("no CRLF at the end");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	TEST_FOOTER;
}