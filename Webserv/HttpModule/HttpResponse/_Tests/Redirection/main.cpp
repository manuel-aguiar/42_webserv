#include "../../HttpResponse.hpp"
#include "../../../HttpRequest/HttpRequest.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../ServerConfig/ServerBlock/ServerBlock.hpp"
#include "../../../../ServerConfig/BlockFinder/BlockFinder.hpp"
#include "../../../../ServerConfig/ServerLocation/ServerLocation.hpp"
#include "../../../../ServerContext/ServerContext.hpp"

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

// Helper function to create a socket address
Ws::Sock::addr_in createSockAddr(const std::string& ip, const std::string& port) {
	Ws::Sock::addr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(static_cast<uint16_t>(std::atoi(port.c_str())));

	if (ip.empty() || ip == "0.0.0.0")
		addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
	else
		inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));

	return addr;
}

void test_basicRedirects(int &testNumber)
{
	TEST_HEADER("HTTP Redirection - Basic Redirect Tests");

	Buffer<1024> buffer;

	// Server block setup (and blockfinder)
	ServerBlock block1;
	BlockFinder finder(200);

	Ws::Sock::addr_in addr = createSockAddr("0.0.0.0", "8080");

	char rootPath[100];
	getcwd(rootPath, 100);
	block1.addListenAddress((Ws::Sock::addr*)&addr);
	block1.addServerName("example.com");
	block1.setRoot(std::string(rootPath) + "/Testfiles");

	// Create a location with a redirect
	block1.accessLocations().push_back(ServerLocation());
	block1.accessLocations().back().setPath("/oldpath");
	block1.accessLocations().back().setRoot(std::string(rootPath) + "/Testfiles");
	block1.accessLocations().back().addMethod("GET");
	block1.accessLocations().back().setReturn("301:/newpath");

	finder.addServerBlock(block1);

	// Server Context
	ServerContext serverContext;
	serverContext.setBlockFinder(finder);

	// Test 301 redirect
	TEST_INTRO(testNumber++);
	{
		Http::Request request(serverContext);
		Http::Response response(serverContext);
		response.setConnectionAddress(*(const Ws::Sock::addr*)&addr);
		request.setResponse(response);

		Buffer<1024> buffer;
		buffer.push("GET /oldpath HTTP/1.1\r\nHost: example.com\r\n\r\n");

		// Parse the request
		request.parse(buffer);

		// Check the response status code
		try {
			// Check if response is in waiting state
			EXPECT_EQUAL(response.getStatus(), Http::ResponseStatus::WAITING, "Response should be waiting to send");

			Http::ResponseData data = response.getResponseData();

			// Check if response data has a redirect status
			EXPECT_EQUAL(data.requestStatus, Http::Status::MOVED_PERMANENTLY, "Status should be 301 Moved Permanently");
			EXPECT_EQUAL(data.responseType, Http::ResponseData::REDIRECT, "Response type should be REDIRECT");
			EXPECT_EQUAL(data.headers["location"], "/newpath", "Redirect location should be /newpath");

			TEST_PASSED_MSG("301 redirect test passed");
		}
		catch(const std::exception& e) {
			TEST_FAILED_MSG(e.what());
		}
	}

	TEST_FOOTER;
}

int main()
{
	int testNumber = 0;
	test_basicRedirects(testNumber);
	return 0;
}
