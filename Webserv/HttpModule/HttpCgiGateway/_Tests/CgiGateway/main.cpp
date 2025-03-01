
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

void invalidScript(int& testNumber)
{
	
	ServerContext 			context;
	Globals 				globals(NULL, NULL, NULL, NULL);
	Events::Manager 		eventManager(30, globals);
	Cgi::Module 			cgi(10, 100, 5000, eventManager, globals);
	
	context.setAddonLayer(Ws::AddonLayer::CGI, &cgi);
	
	try
	{
		TEST_INTRO(testNumber++);
		ServerLocation 		location;
		location.setRoot("./");
		location.addCgiInterpreter(".php:/usr/bin/php");

		Http::RequestData 	requestData;

		requestData.method = "POST";
		requestData.headers.insert(std::pair<std::string, std::string>("Content-Type", "application/x-www-form-urlencoded"));

		Http::ResponseData 	responseData;

		responseData.serverLocation = &location;
		responseData.targetPath = "Scripts/invalid.php";
		responseData.targetExtension = ".php";
		responseData.requestData = &requestData;
		
		Http::CgiGateway 	cgiGateway(cgi);

		EXPECT_EQUAL(cgiGateway.initiateRequest(responseData), true, "CgiGateway::initiateRequest() - success");
		
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

		std::cout << readBuffer.view() << std::endl;

		TEST_PASSED_MSG("Cgi Tests bad script");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
}

void validScript(int& testNumber)
{
	
	ServerContext 			context;
	Globals 				globals(NULL, NULL, NULL, NULL);
	Events::Manager 		eventManager(30, globals);
	Cgi::Module 			cgi(10, 100, 5000, eventManager, globals);
	
	context.setAddonLayer(Ws::AddonLayer::CGI, &cgi);
	
	try
	{
		TEST_INTRO(testNumber++);
		ServerLocation 		location;
		location.setRoot("./");
		location.addCgiInterpreter(".php:/usr/bin/php");

		Http::RequestData 	requestData;

		requestData.method = "POST";
		requestData.headers.insert(std::pair<std::string, std::string>("Content-Type", "application/x-www-form-urlencoded"));

		Http::ResponseData 	responseData;

		responseData.serverLocation = &location;
		responseData.targetPath = "Scripts/valid.php";
		responseData.targetExtension = ".php";
		responseData.requestData = &requestData;
		
		Http::CgiGateway 	cgiGateway(cgi);

		EXPECT_EQUAL(cgiGateway.initiateRequest(responseData), true, "CgiGateway::initiateRequest() - success");
		
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

		std::cout << readBuffer.view() << std::endl;

		TEST_PASSED_MSG("Cgi Tests valid script");
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

	invalidScript(testNumber);
	validScript(testNumber);

	TEST_FOOTER;

	return (0);
}