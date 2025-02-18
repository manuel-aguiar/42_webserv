#include "../../HttpRequest/HttpRequest.hpp"
#include "../HttpResponse.hpp"
#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

#define PATH "/42webserv"

void responseTests(int &testNumber)
{
	Http::Request	httpRequest[30];
	std::string	requestData;

	TEST_HEADER("Http Response");

	TEST_HEADER("Passing Tests");

	////////////////////////////////////////////

	TEST_INTRO(testNumber++);

	ServerContext sc;

	requestData =
		std::string("GET ") + PATH + "/index.html HTTP/1.1\r\n"
		"Host: www.example.com\r\n" +
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n" +
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
		"Accept-Encoding: gzip, deflate, br\r\n" +
		"Accept-Language: en-US,en;q=0.5\r\n" +
		"Connection: keep-alive\r\n" +
		"\r\n";

	httpRequest[testNumber].parse(requestData);
	Http::Response resp0(httpRequest[testNumber], sc);

	////////////////////////////////////////////

	TEST_HEADER("Failing Tests");

	TEST_INTRO(testNumber++);

	requestData =
		std::string("GET /nonexistingfile HTTP/1.1\r\n") +
		"Host: www.example.com\r\n" +
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36\r\n" +
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
		"Accept-Encoding: gzip, deflate, br\r\n" +
		"Accept-Language: en-US,en;q=0.5\r\n" +
		"Connection: keep-alive\r\n" +
		"\r\n";
	
	httpRequest[testNumber].parse(requestData);
	Http::Response resp1(httpRequest[testNumber], sc);

	TEST_FOOTER;
}