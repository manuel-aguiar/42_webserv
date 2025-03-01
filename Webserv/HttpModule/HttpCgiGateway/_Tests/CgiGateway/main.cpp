
#include "../../../HttpResponse/HttpResponse.hpp"
#include "../../../HttpCgiGateway/HttpCgiGateway.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../../../../ServerContext/ServerContext.hpp"
#include "../../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../../ServerConfig/ServerLocation/ServerLocation.hpp"
#include "../../../../Globals/Globals.hpp"
#include "../../../../Events/Manager/Manager.hpp"
#include "../../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
#include "../../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../../../Toolkit/TestHelpers/TestHelpers.h"

void test1(int& testNumber)
{
	TEST_INTRO(testNumber++);

	ServerContext 			context;
	Globals 				globals(NULL, NULL, NULL, NULL);
	Events::Manager 		eventManager(30, globals);
	Cgi::Module 			cgi(10, 100, 5000, eventManager, globals);

	context.setAddonLayer(Ws::AddonLayer::CGI, &cgi);
	
	

	try
	{
		ServerLocation 		location;
		location.setRoot("./");
		location.addCgiInterpreter(".php:/usr/bin/php-cgi");

		Http::RequestData 	requestData;

		requestData.method = "POST";
		requestData.headers.insert(std::pair<std::string, std::string>("Content-Type", "application/x-www-form-urlencoded"));

		Http::ResponseData 	responseData;

		responseData.serverLocation = &location;
		responseData.targetPath = "Scripts/simple.php";
		responseData.targetExtension = ".php";
		responseData.requestData = &requestData;
		
		Http::CgiGateway 	cgiGateway(cgi);

		cgiGateway.initiateRequest(responseData);
		cgiGateway.sendHttpBody(BufferView());

		Buffer<5000> readBuffer;

		while (1)
		{
			int timeout = cgi.processRequests();

			eventManager.ProcessEvents(timeout);
			cgiGateway.fillWriteBuffer(readBuffer);
			if (eventManager.getMonitoringCount() == 0)
				break ;
		}

		

		TEST_PASSED_MSG("Cgi Tests");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
}



int main(void)
{

	TEST_HEADER("Cgi Gateway ");

	int testNumber = 1;

	test1(testNumber);

	TEST_FOOTER;

	return (0);
}