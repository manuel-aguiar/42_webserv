

// Project headers
# include "../../CgiModule.h"

// test helpers
# include "../TestProtoRequest/TestProtoRequest.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../Events/Manager/Manager.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../../../Toolkit/TestHelpers/TestHelpers.h"

//C++ headers
# include <iostream>

// C headers
#include <unistd.h>


extern std::vector<std::string> g_mockGlobals_ErrorMsgs;



void scriptTest(int& testNumber, const std::string& scriptPath, const std::string& interpreterPath, TestProtoRequest::e_CgiResultStatus expected)
{
	// script should run without issue
	try
	{
		TEST_INTRO(testNumber++);

		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 5000, 4096, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		// acquiring a request
		protoRequest.m_CgiRequestData = cgi.acquireRequest();
		
		Cgi::Request& request = *protoRequest.m_CgiRequestData;

		// tell the request who the user of the callbacks is
		request.setUser(&protoRequest);

		// notification callbacks
		request.setNotify_onError(&TestProtoRequest_CgiGateway::onError);
		request.setNotify_onSuccess(&TestProtoRequest_CgiGateway::onSuccess);

		// IO callbacks
		request.setWriteToScript_Callback(&TestProtoRequest_CgiGateway::onWrite);
		request.setReadBodyFromScript_Callback(&TestProtoRequest_CgiGateway::onRead);
		request.setReceiveStatusHeaders_Callback(&TestProtoRequest_CgiGateway::onReceiveHeaders);

		// options, you can ignore this
		request.setRuntimeOptions(Cgi::Options::HOLD_WRITE); // "if write events gets triggered, don't write just yet"
		
		// if the request cannot be completed in 5ms, time out
		request.setTimeoutMs(5000); // 5ms

		// where the script is
		request.setScriptPath(scriptPath);
		request.setInterpreterPath(interpreterPath);
		// set an environment variable that is part of the Cgi specification
		request.setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");

		// set an environment variable that is not part of the Cgi specification, maybe HTTP_SOME_STUFF
		request.setEnvExtra("CUSTOM_ENTRY1", "a random value");

		// place the request for execution. if the user attempts to change execution parameters, the program will ABORT
		cgi.enqueueRequest(request, false);

		// options, don't worry about it, but if you want to change the request, you "can" do it here
		cgi.modifyRequest(request, false, Cgi::Options::RESTART_WRITE);

		//event loop
		while (1)
		{
			// Controller incorporating the CgiModule in its event loop
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getMonitoringCount() != 0)
				// event handler, used by the cgimodule as well as the Conn::Manager, etc
				eventManager.ProcessEvents(nextWait);
			if (cgi.getBusyWorkerCount() == 0)
				break ;
		}

		// no need to call finish request, the CgiModule takes the request as finished.

		// tests
		EXPECT_EQUAL(eventManager.getMonitoringCount(), 0, "Manager still has events");
		EXPECT_EQUAL(cgi.getBusyWorkerCount(), 0, "Cgi::Module still has workers rolling");
		EXPECT_EQUAL(protoRequest.m_CgiResultStatus, expected, "ProtoRequest didn't receive success notice");


		TEST_PASSED_MSG("executing a script");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();
}

void TestPart2(int& testNumber)
{
	scriptTest(testNumber, "../TestScripts/php/ValidPhP.php", "/usr/bin/php", TestProtoRequest::E_CGI_STATUS_SUCCESS);
	scriptTest(testNumber, "../TestScripts/php/ValidLongBody.php", "/usr/bin/php", TestProtoRequest::E_CGI_STATUS_SUCCESS);
	scriptTest(testNumber, "../TestScripts/php/InvalidLongHeaders.php", "/usr/bin/php", TestProtoRequest::E_CGI_STATUS_ERROR);
	scriptTest(testNumber, "../TestScripts/php/InvalidHeaders.php", "/usr/bin/php", TestProtoRequest::E_CGI_STATUS_ERROR);
}