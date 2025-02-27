#include "../../HttpRequest.hpp"
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"

void stateTransitionTests(int &testNumber)
{
	TEST_HEADER("Http Request - State Machine Tests");

	// Initial state tests
	TEST_INTRO(testNumber++);
	{
		ServerContext serverContext;
		Http::Request request(serverContext);
		try {
			EXPECT_EQUAL(request.getParsingState(), Http::Request::IDLE, "Should be in IDLE state initially");
			EXPECT_EQUAL(request.isStarted(), false, "Should not be started initially");
			EXPECT_EQUAL(request.isError(), false, "Should not be in error initially");
			EXPECT_EQUAL(request.isIncompleted(), false, "Should not be incomplete initially");
			EXPECT_EQUAL(request.isCompleted(), false, "Should not be completed initially");
			TEST_PASSED_MSG("Initial state checks passed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	// complete get request
	TEST_INTRO(testNumber++);
	{
		ServerContext serverContext;
		Http::Request request(serverContext);
		Buffer<1024> buffer;
		buffer.push("GET /index.html HTTP/1.1\r\nHost: localhost;\r\n\r\n");
		request.parse(buffer);
		
		try {
		EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Should be completed");
		EXPECT_EQUAL(request.isCompleted(), true, "Should be completed");
		EXPECT_EQUAL(request.getMethod(), "GET", "Should have GET method");
		EXPECT_EQUAL(request.getUri(), "/index.html", "Should have /index.html uri");
		EXPECT_EQUAL(request.getHttpVersion(), "HTTP/1.1", "Should have HTTP/1.1 version");
		EXPECT_EQUAL(request.getHeaders().size(), 1, "Should have 1 (host) header");
		EXPECT_EQUAL(request.getBody(), "", "Should have no body");
		TEST_PASSED_MSG("Complete GET request passed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	// Request line parsing tests
	TEST_INTRO(testNumber++);
	{
		ServerContext serverContext;
		Http::Request request(serverContext);
		try {
			// Partial request line
			Buffer<1024> buffer;
			buffer.push("GET /index.html");
			request.parse(buffer);
			EXPECT_EQUAL(request.isStarted(), true, "Should be started after partial request");
			EXPECT_EQUAL(request.getParsingState(), Http::Request::REQLINE, "Should stay in REQLINE state with partial data");

			// Complete request line by sending only the missing part
			buffer.push(" HTTP/1.1\r\n");  // Only send the missing part
			request.parse(buffer);
			EXPECT_EQUAL(request.getParsingState(), Http::Request::HEADERS, "Should transition to HEADERS state");
			TEST_PASSED_MSG("Request line parsing state transitions passed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	// Headers parsing tests
	TEST_INTRO(testNumber++);
	{
		ServerContext serverContext;
		Http::Request request(serverContext);
		try {
			// Valid request with partial headers
			Buffer<1024> partialBuffer;
			partialBuffer.push(
				"GET /index.html HTTP/1.1\r\n"
				"Host: localhost\r\n"
				"Content-Type: text/plain\r\n"
			);
			request.parse(partialBuffer);
			EXPECT_EQUAL(request.getParsingState(), Http::Request::HEADERS, "Should stay in HEADERS state with partial headers");

			// Complete headers
			Buffer<1024> completeBuffer;

			// pushing the remaining
			completeBuffer.push(
				"\r\n"
			);
			request.parse(completeBuffer);
			EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Should transition to COMPLETED state");
			TEST_PASSED_MSG("Headers parsing state transitions passed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	// Body parsing tests (Content-Length)
	TEST_INTRO(testNumber++);
	{
		ServerContext serverContext;
		Http::Request request(serverContext);
		try {
			Buffer<1024> buffer;
			buffer.push(
				"POST /submit HTTP/1.1\r\n"
				"Host: localhost\r\n"
				"Content-Length: 11\r\n\r\n"
				"Hello World"
			);
			request.parse(buffer);
			EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Should be COMPLETED with full body");
			EXPECT_EQUAL(request.isCompleted(), true, "Should be marked as completed");
			TEST_PASSED_MSG("Content-Length body parsing state transitions passed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	// Chunked body parsing tests
	// TEST_INTRO(testNumber++);
	// {
	// 	ServerContext serverContext;
	// Http::Response response;
	//	Http::Request request(s
	// request.setResponse(response);erverContext);
	// 	try {
	// 		// Chunked request with partial chunk
	// 		Buffer<1024> partialBuffer;
	// 		partialBuffer.push(
	// 			"POST /submit HTTP/1.1\r\n"
	// 			"Host: localhost\r\n"
	// 			"Transfer-Encoding: chunked\r\n\r\n"
	// 			"5\r\nHello\r\n"
	// 		);
	// 		request.parse(partialBuffer);
	// 		EXPECT_EQUAL(request.isIncompleted(), true, "Should be incomplete with partial chunks");

	// 		// Complete chunked request
	// 		Buffer<1024> completeBuffer;
	// 		completeBuffer.push(
	// 			"POST /submit HTTP/1.1\r\n"
	// 			"Host: localhost\r\n"
	// 			"Transfer-Encoding: chunked\r\n\r\n"
	// 			"5\r\nHello\r\n"
	// 			"0\r\n\r\n"
	// 		);
	// 		request.parse(completeBuffer);
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Should be COMPLETED with final chunk");
	// 		TEST_PASSED_MSG("Chunked body parsing state transitions passed");
	// 	}
	// 	catch(const std::exception& e) {
	// 		TEST_FAILED_MSG(e.what());
	// 	}
	// }

	// Error state tests
	TEST_INTRO(testNumber++);
	{
		ServerContext serverContext;
		Http::Request request(serverContext);
		try {
			// Invalid request line
			Buffer<1024> buffer;
			buffer.push("INVALID /index.html HTTP/1.1\r\n");
			request.parse(buffer);
			EXPECT_EQUAL(request.isError(), true, "Should be in error state with invalid request");
			EXPECT_EQUAL(request.getParsingState(), Http::Request::ERROR, "Should be in ERROR state");

			// Reset test
			request.reset();
			EXPECT_EQUAL(request.getParsingState(), Http::Request::IDLE, "Should return to IDLE after reset");
			EXPECT_EQUAL(request.isError(), false, "Should not be in error after reset");
			TEST_PASSED_MSG("Error state and reset handling passed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	TEST_FOOTER;
}

int main()
{
	int testNumber = 1;

	std::cout << "Starting HTTP Request Parser Tests...\n\n";

	stateTransitionTests(testNumber);

	std::cout << "\nAll tests completed!\n";
	return 0;
}