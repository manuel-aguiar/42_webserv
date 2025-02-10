

// Project headers
# include "../../CgiModule.h"

# include "CgiStressTest.hpp"

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

/*
	General tests to check if the environment variables are correctly passed
	to the scripts.

	Same as the previous tests but adding environment variables and checking
	if the cleanup is done properly
*/

void TestPart2(int& testNumber)
{
	// executing a script, no environment variables so far
	// script should run without issue
	try
	{
		TEST_INTRO(testNumber++);

		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 5000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);

		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_ERROR_RUNTIME, &TestProtoRequest_CgiGateway::onErrorRuntime);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_ERROR_STARTUP, &TestProtoRequest_CgiGateway::onErrorStartup);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_ERROR_TIMEOUT, &TestProtoRequest_CgiGateway::onErrorTimeOut);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000);
		

		
		///////////////// Setting Variables ////////////////////

		// normal ones
		protoRequest.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::CONTENT_LENGTH, "123");

		// custom
		protoRequest.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");

		// doubled entry, the first should prevail
		protoRequest.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "DoubledBasic");


		////////////////////////////////////////////////////////////////

		CgiStressTest::prepareExpectedOutput(true, protoRequest);

		cgi.enqueueRequest(*protoRequest.m_CgiRequestData, false);

		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getMonitoringCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
		}

		// tests
		EXPECT_EQUAL(eventManager.getMonitoringCount(), 0, "Manager still has events");
		EXPECT_EQUAL(cgi.getBusyWorkerCount(), 0, "Cgi::Module still has workers rolling");
		EXPECT_EQUAL(protoRequest.m_CgiResultStatus, TestProtoRequest::E_CGI_STATUS_SUCCESS, "Cgi result status mismatch");
		EXPECT_EQUAL(protoRequest.m_TotalBytesRead, protoRequest.m_ExpectedOutput.length(), "Length doesn't match");
		EXPECT_EQUAL(std::string(protoRequest.m_buffer), protoRequest.m_ExpectedOutput, "Script output doesn't match expected");

		TEST_PASSED_MSG("env vars + testing env variables");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

}