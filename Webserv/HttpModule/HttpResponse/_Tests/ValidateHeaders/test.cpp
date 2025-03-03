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
#include <cstdio>

// Test Variable from mockHttpResponse_ValidateHeaders.cpp
extern bool g_returnValue;
extern int	g_requestStatus;


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

void test_simpleRequests(int &testNumber)
{
	TEST_HEADER("Http Response - mf_validateHeaders() - Simple Requests");
	Buffer<1024> buffer;

	// Server block setup (and blockfinder)
	ServerBlock block1;
	BlockFinder finder(1 + 1);


	Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0.0", "8080");
	
	char rootPath[100];
	getcwd(rootPath, 100);
	block1.addListenAddress((Ws::Sock::addr*)&addr1);
	block1.addServerName("example.com");
	block1.setRoot(std::string(rootPath) + "/Testfiles");
	block1.accessLocations().push_back(ServerLocation());
	block1.accessLocations().back().setPath("/");
	
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("index.html");
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("index.html");
		buffer.push("GET / HTTP/1.1\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("index.html");
		buffer.push("GET / HTTP/1.1\r\nAccept: application/octet-stream\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::NOT_ACCEPTABLE, "RequestStatus should be 406 NOT ACCEPTABLE");
			EXPECT_EQUAL(g_returnValue, false, "Return value should be unsuccessful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("index.html");
		buffer.push("GET /doesntexist.html HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::NOT_FOUND, "RequestStatus should be 404 NOT FOUND");
			EXPECT_EQUAL(g_returnValue, false, "Return value should be unsuccessful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("index.html");
		buffer.push("GET /doesntexist/ HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::NOT_FOUND, "RequestStatus should be 404 NOT FOUND");
			EXPECT_EQUAL(g_returnValue, false, "Return value should be unsuccessful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();

		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::METHOD_NOT_ALLOWED, "RequestStatus should be 405 METHOD NOT ALLOWED");
			EXPECT_EQUAL(g_returnValue, false, "Return value should be unsuccessful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();

		buffer.push("POST / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::METHOD_NOT_ALLOWED, "RequestStatus should be 405 METHOD NOT ALLOWED");
			EXPECT_EQUAL(g_returnValue, false, "Return value should be unsuccessful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		buffer.push("DELETE / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::METHOD_NOT_ALLOWED, "RequestStatus should be 405 METHOD NOT ALLOWED");
			EXPECT_EQUAL(g_returnValue, false, "Return value should be unsuccessful");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().setIndex("");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		buffer.push("GET /dir/ HTTP/1.1\r\nHost: example.com\r\n\r\n");

		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::FORBIDDEN, "RequestStatus should be 403 FORBIDDEN");
			EXPECT_EQUAL(g_returnValue, false, "Return value should be unsuccessful");
			TEST_PASSED_MSG("Directory GET request with auto index off");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////
	
	// Directory GET with no location
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("1");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		buffer.push("GET /dir/ HTTP/1.1\r\nHost: example.com\r\n\r\n");

		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Directory GET request with auto index on");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// Directory GET + Index
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("1");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("index.html");
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");

		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Directory GET request with auto index on, and index file. Should get index file.");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// Directory GET + Non existent Index
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("1");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("doesntexist.html");
		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");

		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Directory GET request with auto index on, and non existent index file");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().setIndex("index.html");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("POST");
		buffer.push("POST /file.php HTTP/1.1\r\nHost: example.com\r\nContent-Length: 42\r\n\r\nname=John+Doe&email=john.doe%40example.com\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Html file POST request");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// DELETE
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		buffer.clear();
		
		// TESTED Request Parameters
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("DELETE");
		buffer.push("DELETE /todelete.txt HTTP/1.1\r\nHost: example.com\r\n\r\n");

		std::string filePath = std::string(rootPath) + "/Testfiles/todelete.txt";
		
		// Create file to delete
		FILE *file = fopen(filePath.c_str(), "w");
		fclose(file);

		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::NO_CONTENT, "RequestStatus should be 204 NO CONTENT");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Html file DELETE request");
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
		block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
		block1.accessLocations().back().setAutoindex("0");
		block1.accessLocations().back().clearMethods();
		block1.accessLocations().back().addMethod("GET");
		block1.accessLocations().back().setIndex("index.html");

		buffer.push("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Html file GET request (only host header)");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////

	// Edge Case - No location
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		block1.accessLocations().clear();

		buffer.push("GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("Edge Case - No location");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

}

void test_complexLoctaions(int &testNumber)
{
	TEST_HEADER("Http Response - mf_validateHeaders() - Complex Locations");

	Buffer<1024> buffer;

	// Server block setup (and blockfinder)
	ServerBlock block1;
	BlockFinder finder(1 + 1);


	Ws::Sock::addr_in addr1 = createSockAddr("0.0.0.0.0", "8080");
	
	char rootPath[100];
	getcwd(rootPath, 100);
	block1.addListenAddress((Ws::Sock::addr*)&addr1);
	block1.addServerName("example.com");
	block1.setRoot(std::string(rootPath) + "/Testfiles");

	// Location /	
	block1.accessLocations().push_back(ServerLocation());
	block1.accessLocations().back().setPath("/");
	block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
	block1.accessLocations().back().setAutoindex("0");
	block1.accessLocations().back().addMethod("GET");
	block1.accessLocations().back().setIndex("index.html");

	// Location /three
	block1.accessLocations().push_back(ServerLocation());
	block1.accessLocations().back().setPath("/three");
	block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles/one/two/three/four");
	block1.accessLocations().back().setAutoindex("1");
	block1.accessLocations().back().addMethod("GET");
	block1.accessLocations().back().setIndex("file.txt");
	
	// Location /three/four
	block1.accessLocations().push_back(ServerLocation());
	block1.accessLocations().back().setPath("/three/four");
	block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles/one/two/three/four");
	block1.accessLocations().back().setAutoindex("1");
	block1.accessLocations().back().addMethod("GET");
	block1.accessLocations().back().setIndex("nonexistent.txt");


	finder.addServerBlock(block1);

	// Sever Context
	ServerContext serverContext;
	serverContext.setBlockFinder(finder);

	// Response & request
	
	/////////////////////////////////////////////////////
	// TESTS
	/////////////////////////////////////////////////////

	// Access specific loctaion /three
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		buffer.push("GET /three/ HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("GET request to /three");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////


	// Access specific loctaion /three/four
	TEST_INTRO(testNumber++);
	{
		Http::Request	request(serverContext);
		Http::Response	response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr1);
		request.setResponse(response);

		Buffer<1024> buffer;
		
		// TESTED Request Parameters
		buffer.push("GET /three/four/file.txt HTTP/1.1\r\nHost: example.com\r\n\r\n");
		
		request.parse(buffer);

		try {
			EXPECT_EQUAL(g_requestStatus, Http::Status::OK, "RequestStatus should be 200 OK");
			EXPECT_EQUAL(g_returnValue, true, "Return value should be successful");
			TEST_PASSED_MSG("GET request to /three/four/file.txt");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	/////////////////////////////////////////////////////


}


int main()
{
	int testNumber = 1;

	TEST_HEADER("Http Response - mf_validateHeaders()");

	test_simpleRequests(testNumber);
	test_complexLoctaions(testNumber);
	
	TEST_FOOTER;

	return 0;
}