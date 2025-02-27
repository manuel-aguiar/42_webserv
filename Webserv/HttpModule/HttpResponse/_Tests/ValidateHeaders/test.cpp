#include "../../HttpResponse.hpp"
#include "../../../HttpRequest/HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../../../ServerConfig/ServerLocation/ServerLocation.hpp"
#include "../../../../ServerContext/ServerContext.hpp"


#include <iomanip>
#include <arpa/inet.h>
#include <cstdlib>

// Test Variable from mockHttpResponse_ValidateHeaders.cpp
extern bool returnValue;
extern int	requestStatus;


// Tester function
Ws::Sock::addr_in createSockAddr(const std::string& ip, const std::string& port) {
	Ws::Sock::addr_in addr = (Ws::Sock::addr_in){};
	addr.sin_family = AF_INET;

	// Convert port string to number (simple atoi for testing)
	addr.sin_port = htons(std::atoi(port.c_str()));

	// Handle IP
	if (ip.empty() || ip == "0.0.0.0")
		addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
	else
		inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));

	return addr;
}

void simpleRequests(int &testNumber)
{
	TEST_HEADER("Http Response - mf_validateHeaders()");

	// Server block setup (and blockfinder)
	ServerBlock block1;
	BlockFinder finder;

	Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0.0", "8080");
	
	block1.addListenAddress((Ws::Sock::addr*)&addr1);
	block1.addServerName("example.com");
	block1.accessLocations().push_back(ServerLocation());
	block1.accessLocations().back().setPath("/");
	char rootPath[100];
	
	finder.addServerBlock(block1);

	// Sever Context
	ServerContext serverContext;
	serverContext.setBlockFinder(finder);

	// Response & request
	
	/////////////////////////////////////////////////////
	// TESTS
	/////////////////////////////////////////////////////

	// File GET
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/index.html");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Html file GET request");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// File GET
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/doesntexist");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::NOT_FOUND, "RequestStatus should be 404 NOT FOUND");
			EXPECT_EQUAL(returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("Inexistent file GET request");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// File GET
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/doesntexist");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		buffer.push("GET /doesntexist HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::NOT_FOUND, "RequestStatus should be 404 NOT FOUND");
			EXPECT_EQUAL(returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("Inexistent location GET request");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// File GET
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/index.html");
		block1.accessLocations().back().clearMethods();
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::METHOD_NOT_ALLOWED, "RequestStatus should be 405 METHOD NOT ALLOWED");
			EXPECT_EQUAL(returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("GET request not allowed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	
	// Directory GET
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setAutoindex("0");
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");

		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::FORBIDDEN, "RequestStatus should be 403 FORBIDDEN");
			EXPECT_EQUAL(returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("Directory GET request with auto index off");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	
	// Directory GET
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setAutoindex("1");
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");

		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Directory GET request with auto index on");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// POST
	// TEST_INTRO(testNumber++);
	// {
	// 	Http::Request	request(serverContext);
	// 	Http::Response	response(serverContext);
	// 	response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
	// 	request.setResponse(response);

	// 	Buffer<1024> buffer;
		
	// 	// TESTED Request Parameters
	// 	block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/index.html");
	// 	block1.accessLocations().back().clearMethods();
	// 	block1.accessLocations().back().addMethod("POST");
	// 	buffer.push("POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 42\r\n\r\nname=John+Doe&email=john.doe%40example.com\r\n");
		
	// 	request.parse(buffer);

	// 	try {
	// 		EXPECT_EQUAL(requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
	// 		EXPECT_EQUAL(returnValue, true, "Return value should be successful");
	// 		TEST_PASSED_MSG("Html file POST request");
	// 	}
	// 	catch(const std::exception& e) {
	// 		TEST_FAILED_MSG(e.what());
	// 	}
	// }

	/////////////////////////////////////////////////////











	// // Request line parsing tests
	// TEST_INTRO(testNumber++);
	// {
	// 	ServerContext serverContext;
	// 	Http::Request request(serverContext);
	// 	try {
	// 		// Partial request line
	// 		Buffer<1024> buffer;
	// 		buffer.push("GET /index.html");
	// 		request.parse(buffer);
	// 		EXPECT_EQUAL(request.isStarted(), true, "Should be started after partial request");
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::REQLINE, "Should stay in REQLINE state with partial data");

	// 		// Complete request line by sending only the missing part
	// 		buffer.push(" HTTP/1.1\r\n");  // Only send the missing part
	// 		request.parse(buffer);
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::HEADERS, "Should transition to HEADERS state");
	// 		TEST_PASSED_MSG("Request line parsing state transitions passed");
	// 	}
	// 	catch(const std::exception& e) {
	// 		TEST_FAILED_MSG(e.what());
	// 	}
	// }

	// // Headers parsing tests
	// TEST_INTRO(testNumber++);
	// {
	// 	ServerContext serverContext;
	// 	Http::Request request(serverContext);
	// 	try {
	// 		// Valid request with partial headers
	// 		Buffer<1024> partialBuffer;
	// 		partialBuffer.push(
	// 			"GET /index.html HTTP/1.1\r\n"
	// 			"Host: localhost\r\n"
	// 			"Content-Type: text/plain\r\n"
	// 		);
	// 		request.parse(partialBuffer);
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::HEADERS, "Should stay in HEADERS state with partial headers");

	// 		// Complete headers
	// 		Buffer<1024> completeBuffer;

	// 		// pushing the remaining
	// 		completeBuffer.push(
	// 			"\r\n"
	// 		);
	// 		request.parse(completeBuffer);
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Should transition to COMPLETED state");
	// 		TEST_PASSED_MSG("Headers parsing state transitions passed");
	// 	}
	// 	catch(const std::exception& e) {
	// 		TEST_FAILED_MSG(e.what());
	// 	}
	// }

	// // Body parsing tests (Content-Length)
	// TEST_INTRO(testNumber++);
	// {
	// 	ServerContext serverContext;
	// 	Http::Request request(serverContext);
	// 	try {
	// 		Buffer<1024> buffer;
	// 		buffer.push(
	// 			"POST /submit HTTP/1.1\r\n"
	// 			"Host: localhost\r\n"
	// 			"Content-Length: 11\r\n\r\n"
	// 			"Hello World"
	// 		);
	// 		request.parse(buffer);
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Should be COMPLETED with full body");
	// 		EXPECT_EQUAL(request.isCompleted(), true, "Should be marked as completed");
	// 		TEST_PASSED_MSG("Content-Length body parsing state transitions passed");
	// 	}
	// 	catch(const std::exception& e) {
	// 		TEST_FAILED_MSG(e.what());
	// 	}
	// }

	// // Chunked body parsing tests
	// // TEST_INTRO(testNumber++);
	// // {
	// // 	ServerContext serverContext;
	// // Http::Response response;
	// //	Http::Request request(s
	// // request.setResponse(response);erverContext);
	// // 	try {
	// // 		// Chunked request with partial chunk
	// // 		Buffer<1024> partialBuffer;
	// // 		partialBuffer.push(
	// // 			"POST /submit HTTP/1.1\r\n"
	// // 			"Host: localhost\r\n"
	// // 			"Transfer-Encoding: chunked\r\n\r\n"
	// // 			"5\r\nHello\r\n"
	// // 		);
	// // 		request.parse(partialBuffer);
	// // 		EXPECT_EQUAL(request.isIncompleted(), true, "Should be incomplete with partial chunks");

	// // 		// Complete chunked request
	// // 		Buffer<1024> completeBuffer;
	// // 		completeBuffer.push(
	// // 			"POST /submit HTTP/1.1\r\n"
	// // 			"Host: localhost\r\n"
	// // 			"Transfer-Encoding: chunked\r\n\r\n"
	// // 			"5\r\nHello\r\n"
	// // 			"0\r\n\r\n"
	// // 		);
	// // 		request.parse(completeBuffer);
	// // 		EXPECT_EQUAL(request.getParsingState(), Http::Request::COMPLETED, "Should be COMPLETED with final chunk");
	// // 		TEST_PASSED_MSG("Chunked body parsing state transitions passed");
	// // 	}
	// // 	catch(const std::exception& e) {
	// // 		TEST_FAILED_MSG(e.what());
	// // 	}
	// // }

	// // Error state tests
	// TEST_INTRO(testNumber++);
	// {
	// 	ServerContext serverContext;
	// 	Http::Request request(serverContext);
	// 	try {
	// 		// Invalid request line
	// 		Buffer<1024> buffer;
	// 		buffer.push("INVALID /index.html HTTP/1.1\r\n");
	// 		request.parse(buffer);
	// 		EXPECT_EQUAL(request.isError(), true, "Should be in error state with invalid request");
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::ERROR, "Should be in ERROR state");

	// 		// Reset test
	// 		request.reset();
	// 		EXPECT_EQUAL(request.getParsingState(), Http::Request::IDLE, "Should return to IDLE after reset");
	// 		EXPECT_EQUAL(request.isError(), false, "Should not be in error after reset");
	// 		TEST_PASSED_MSG("Error state and reset handling passed");
	// 	}
	// 	catch(const std::exception& e) {
	// 		TEST_FAILED_MSG(e.what());
	// 	}
	// }

	TEST_FOOTER;
}

int main()
{
	int testNumber = 1;

	simpleRequests(testNumber);

	return 0;
}