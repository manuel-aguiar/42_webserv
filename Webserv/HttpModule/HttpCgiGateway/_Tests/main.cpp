
#include "../../HttpResponse/HttpResponse.hpp"
#include "../../HttpCgiGateway/HttpCgiGateway.hpp"
#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

#include "../../../ServerContext/ServerContext.hpp"
#include "../../../GenericUtils/Buffer/Buffer.hpp"
#include "../../../GenericUtils/Buffer/HeapBuffer.hpp"
#include "../../../GenericUtils/BufferView/BufferView.hpp"
#include "../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../ServerConfig/ServerLocation/ServerLocation.hpp"
#include "../../../Globals/Globals.hpp"
#include "../../../Events/Manager/Manager.hpp"
#include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
#include "../../../GenericUtils/StringUtils/StringUtils.hpp"
#include "../../../SignalHandler/SignalHandler.hpp"
#include "../../../../Toolkit/TestHelpers/TestHelpers.h"

# include <signal.h>

void invalidScript(int& testNumber)
{
	
	ServerContext 			context;
	Globals 				globals(NULL, NULL, NULL, NULL);
	Events::Manager 		eventManager(30, globals);
	Cgi::Module 			cgi(10, 100, 5000, eventManager, globals);

	g_SignalHandler.ignoreSignal(SIGPIPE, NULL);

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
		
		BufferView mustSend = cgiGateway.sendHttpBody(BufferView("adsghasgasgasgasgasgasgasgasgasgasg"));

		Buffer<5000> readBuffer;

		while (1)
		{
			int timeout = cgi.processRequests();
			eventManager.ProcessEvents(timeout);
			cgiGateway.fillWriteBuffer(readBuffer);
			mustSend = cgiGateway.sendHttpBody(mustSend);
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

void phpCgi_validScript(int& testNumber)
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
		location.addCgiInterpreter(".php:/usr/bin/php-cgi");

		Http::RequestData 	requestData;

		requestData.method = "POST";
		requestData.headers.insert(std::pair<std::string, std::string>("Content-Type", "application/x-www-form-urlencoded"));

		Http::ResponseData 	responseData;

		responseData.serverLocation = &location;
		responseData.targetPath = "Scripts/phpCgi_valid.php";
		responseData.targetExtension = ".php";
		responseData.requestData = &requestData;
		
		Http::CgiGateway 	cgiGateway(cgi);

		EXPECT_EQUAL(cgiGateway.initiateRequest(responseData), true, "CgiGateway::initiateRequest() - success");
		
		
		cgiGateway.sendHttpBody(BufferView("some stupid message body that must be sent hurray"));

		Buffer<5000> readBuffer;

		while (1)
		{
			int timeout = cgi.processRequests();
			eventManager.ProcessEvents(timeout);
			cgiGateway.fillWriteBuffer(readBuffer);
			cgiGateway.sendHttpBody(BufferView()); // EOF
			if (eventManager.getMonitoringCount() == 0)
				break ;
		}

		std::cout << readBuffer.view() << std::endl;

		TEST_PASSED_MSG("Cgi Tests valid php-cgi");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
}

void php_validScript(int& testNumber)
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
		responseData.targetPath = "Scripts/php_valid.php";
		responseData.targetExtension = ".php";
		responseData.requestData = &requestData;
		
		Http::CgiGateway 	cgiGateway(cgi);

		EXPECT_EQUAL(cgiGateway.initiateRequest(responseData), true, "CgiGateway::initiateRequest() - success");
		
		

		Buffer<5000> readBuffer;

		while (1)
		{
			int timeout = cgi.processRequests();
			eventManager.ProcessEvents(timeout);
			cgiGateway.fillWriteBuffer(readBuffer);

			cgiGateway.sendHttpBody(BufferView("message bodiiiiiiii nobody cares about this but yoyoyoyoyoyo"));
			cgiGateway.sendHttpBody(BufferView()); //EOF, will be ignored hereafter

			if (eventManager.getMonitoringCount() == 0)
				break ;
		}

		std::cout << readBuffer.view() << std::endl;

		TEST_PASSED_MSG("Cgi Tests valid php");
	}
	catch(const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
}

void py_validScript(int& testNumber)
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
		location.addCgiInterpreter(".php:/usr/bin/python3");

		Http::RequestData 	requestData;

		requestData.method = "POST";
		requestData.headers.insert(std::pair<std::string, std::string>("Content-Type", "application/x-www-form-urlencoded"));

		Http::ResponseData 	responseData;

		responseData.serverLocation = &location;
		responseData.targetPath = "Scripts/py_valid.py";
		responseData.targetExtension = ".php";
		responseData.requestData = &requestData;
		
		Http::CgiGateway 	cgiGateway(cgi);

		EXPECT_EQUAL(cgiGateway.initiateRequest(responseData), true, "CgiGateway::initiateRequest() - success");
		
		cgiGateway.sendHttpBody(BufferView("somestupidmessagebodythatmustbesent"));

		Buffer<5000> readBuffer;

		while (1)
		{
			int timeout = cgi.processRequests();
			eventManager.ProcessEvents(timeout);
			cgiGateway.fillWriteBuffer(readBuffer);

			cgiGateway.sendHttpBody(BufferView()); // EOF

			if (eventManager.getMonitoringCount() == 0)
				break ;
		}

		std::cout << readBuffer.view() << std::endl;

		TEST_PASSED_MSG("Cgi Tests valid python");
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
	phpCgi_validScript(testNumber);
	php_validScript(testNumber);
	py_validScript(testNumber);

	TEST_FOOTER;

	return (0);
}