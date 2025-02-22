#include "../../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../Globals/Globals.hpp"
#include "../../HttpRequest/HttpRequest.hpp"
#include "../../HttpModule/HttpModule.hpp"
#include "../HttpConnection/HttpConnection.hpp"
#include "../HttpTransaction/HttpTransaction.hpp"
#include "../HttpResponse.hpp"


#define PATH "/42webserv"

void responseTests(int &testNumber)
{
	Globals globals(NULL, NULL, NULL, NULL);
	Http::Module module(10, globals);
	Http::Connection conn(module);
	Http::Transaction tr(conn);
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

	tr.request.parse(requestData);
	
	////////////////////////////////////////////

	TEST_HEADER("Failing Tests");

	TEST_INTRO(testNumber++);


	TEST_FOOTER;
}