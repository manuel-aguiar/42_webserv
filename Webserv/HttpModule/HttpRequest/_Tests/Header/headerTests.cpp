#include "../../HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void headerTests(int &testNumber)
{
	TEST_HEADER("Http Request - Header");

	TEST_HEADER("Passing Tests");

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n" +
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
			"Accept-Encoding: gzip, deflate, br\r\n" +
			"Accept-Language: en-US,en;q=0.5\r\n" +
			"Connection: keep-alive\r\n" +
			"\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::OK, "Should pass");
			TEST_PASSED_MSG("Common request");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	TEST_HEADER("Failing Tests");

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("No headers");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			":\r\n" +
			"\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with no key nor value");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			": Great value\r\n" +
			"\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with no key");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"    : Great value\r\n" +
			"\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with space as key");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"Great-Key:\r\n" +
			"\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with no value");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n" +
			"Great-Key:    \r\n" +
			"\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("header with space as value");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	{
		TEST_INTRO(testNumber++);
		HttpRequest httpRequest;
		std::string requestData =
			std::string("GET /index.html HTTP/1.1\r\n") +
			"Host: www.example.com\r\n";
		try
		{
			EXPECT_EQUAL(httpRequest.parse(requestData), (int)Http::Status::BAD_REQUEST, "Should fail");
			TEST_PASSED_MSG("no CRLF at the end");
		}
		catch(const std::exception& e)
		{
			TEST_FAILED_MSG(e.what());
		}
	}

	TEST_FOOTER;
}