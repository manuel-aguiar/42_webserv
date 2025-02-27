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

	Buffer<1024> buffer;

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
			TEST_PASSED_MSG("Html file GET request (only host header)");
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
		block1.accessLocations().back().addMethod("GET");
		buffer.push("GET / HTTP/1.1\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Html file GET request with accept headers");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

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
		buffer.push("GET / HTTP/1.1\r\nAccept: application/octet-stream\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::NOT_ACCEPTABLE, "RequestStatus should be 406 NOT ACCEPTABLE");
			EXPECT_EQUAL(returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("Html file GET request with application/octet-stream as only accept type");
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

		buffer.clear();
		
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

		buffer.clear();
		
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

	// GET Method Not Allowed
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
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

	// POST Method Not Allowed
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/index.html");
		block1.accessLocations().back().clearMethods();

		buffer.push("POST / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::METHOD_NOT_ALLOWED, "RequestStatus should be 405 METHOD NOT ALLOWED");
			EXPECT_EQUAL(returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("POST request not allowed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	// /////////////////////////////////////////////////////

	// DELETE Method Not Allowed
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/index.html");
		block1.accessLocations().back().clearMethods();
		buffer.push("DELETE / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::METHOD_NOT_ALLOWED, "RequestStatus should be 405 METHOD NOT ALLOWED");
			EXPECT_EQUAL(returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("DELETE request not allowed");
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

		buffer.clear();
		
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

		buffer.clear();
		
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
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(getcwd(rootPath, 100)) + "/Testfiles/index.html");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("POST");
		buffer.push("POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 42\r\n\r\nname=John+Doe&email=john.doe%40example.com\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Html file POST request");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	TEST_FOOTER;
}

int main()
{
	int testNumber = 1;

	simpleRequests(testNumber);

	return 0;
}