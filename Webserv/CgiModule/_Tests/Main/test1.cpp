

// Project headers
# include "../../CgiModule.h"

# include "CgiStressTest.hpp"

// test helpers
# include "../TestProtoRequest/TestProtoRequest.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../ServerManager/EventManager/EventManager/EventManager.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../../../Toolkit/_Tests/test.h"

//C++ headers
# include <iostream>

// C headers
#include <unistd.h>



extern std::vector<std::string> g_mockGlobals_ErrorMsgs;

int TestPart1(int testNumber)
{
/******************************************************* */
/******************************************************* */
	//instantiation and cleanup test	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(10, 100, 1000, eventManager, globals);				// 10 workers, 100 backlog

		std::cout << "	PASSED (instantiation and cleanup)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

	//instantiation and cleanup test	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);

		Cgi::Module cgi(10, 100, 1000, eventManager, globals);				// 10 workers, 100 backlog
		Cgi::Request* data = cgi.acquireRequest();
		(void)data;
		std::cout << "	PASSED (acquiring and just going away)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

	//instantiation and cleanup test	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(10, 100, 1000, eventManager, globals);				// 10 workers, 100 backlog

		cgi.processRequests();
		std::cout << "	PASSED (finish timeout with empty queues)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

/***************************************************************************** */	
	// script should run without issue
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(10, 100, 5000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequest();
		
		Cgi::Request& request = *protoRequest.m_CgiRequestData;

		// preparing request with relevant data
		request.setUser(&protoRequest);
		request.setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_RUNTIME, &TestProtoRequest_CgiGateway::onErrorRuntime);
		request.setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_STARTUP, &TestProtoRequest_CgiGateway::onErrorStartup);
		request.setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_TIMEOUT, &TestProtoRequest_CgiGateway::onErrorTimeOut);
		request.setRuntime_Callback(CgiRuntime_Callback::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		request.setIO_Callback(CgiIO_Callback::READ, &TestProtoRequest_CgiGateway::onRead);
		request.setIO_Callback(CgiIO_Callback::WRITE, &TestProtoRequest_CgiGateway::onWrite);
		request.setRuntimeOptions(CgiOptions::HOLD_WRITE);
		request.setTimeoutMs(5000); // 5ms
		request.setExtension("py");
		request.setScriptPath("TestScripts/py/envPrint.py");
		request.setEnvBase(CgiEnvEnum::AUTH_TYPE, "Basic");

		CgiStressTest::prepareExpectedOutput(true, protoRequest);

		cgi.enqueueRequest(request, false);
		cgi.modifyRequest(request, CgiOptions::RESTART_WRITE, false);

		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getSubscribeCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
		}


		// tests
		if (eventManager.getSubscribeCount() != 0)
			throw std::runtime_error("EventManager still has events, got " + StringUtils::to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));	

		if (cgi.getBusyWorkerCount() != 0)
			throw std::runtime_error("Cgi::Module still has workers rolling, got " + StringUtils::to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (protoRequest.m_TotalBytesRead != protoRequest.m_ExpectedOutput.length() ||
			std::string(protoRequest.m_buffer) != protoRequest.m_ExpectedOutput)
			throw std::logic_error("Script output doesn't match expected\n\ngot:\n\n" + std::string(protoRequest.m_buffer) + "\nexpected:\n\n" 
			+ protoRequest.m_ExpectedOutput + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (executing a script)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();


/****************************************************** */
	// Testing timeout cleanup
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(10, 100, 500, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_RUNTIME, &TestProtoRequest_CgiGateway::onErrorRuntime);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_STARTUP, &TestProtoRequest_CgiGateway::onErrorStartup);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_TIMEOUT, &TestProtoRequest_CgiGateway::onErrorTimeOut);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

		
		
		protoRequest.m_CgiRequestData->setEnvBase(CgiEnvEnum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setTimeoutMs(200); //0.2ms
		
		// false, we will cancel
		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		cgi.enqueueRequest(*protoRequest.m_CgiRequestData, false);

		::usleep(250000); // 0.25ms

		//event loop
		

		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getSubscribeCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
		}


		// tests
		if (eventManager.getSubscribeCount() != 0)
			throw std::runtime_error("EventManager still has events, got " + StringUtils::to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));	

		if (cgi.getBusyWorkerCount() != 0)
			throw std::runtime_error("Cgi::Module still has workers rolling, got " + StringUtils::to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (protoRequest.m_CgiResultStatus != TestProtoRequest::E_CGI_STATUS_TIMEOUT)
			throw std::logic_error("ProtoRequest didn't receive timeout notice " + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (protoRequest.m_TotalBytesRead != protoRequest.m_ExpectedOutput.length() ||
			std::string(protoRequest.m_buffer) != protoRequest.m_ExpectedOutput)
			throw std::logic_error("Script output doesn't match expected\n\ngot:\n\n" + std::string(protoRequest.m_buffer) + "\nexpected:\n\n" 
			+ protoRequest.m_ExpectedOutput + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (finishing requests that timed out)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();


// Test passing a bad interpreter to execve, leading to failure
	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::string		testFailure;
		g_mockGlobals_ErrorMsgs.clear();

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(10, 100, 5000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "potato");
		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);

		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_RUNTIME, &TestProtoRequest_CgiGateway::onErrorRuntime);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_STARTUP, &TestProtoRequest_CgiGateway::onErrorStartup);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_TIMEOUT, &TestProtoRequest_CgiGateway::onErrorTimeOut);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

		
		protoRequest.m_CgiRequestData->setEnvBase(CgiEnvEnum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000); //0.2ms

		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		cgi.enqueueRequest(*protoRequest.m_CgiRequestData, false);


		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getSubscribeCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
		}


		// tests
		if (eventManager.getSubscribeCount() != 0)
			throw std::runtime_error("EventManager still has events, got " + StringUtils::to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (cgi.getBusyWorkerCount() != 0)
			throw std::runtime_error("Cgi::Module still has workers rolling, got " + StringUtils::to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::string expectedError("InternalCgiWorker::mf_executeChild(), execve(): No such file or directory");

		if (g_mockGlobals_ErrorMsgs.size() != 1)
			throw std::runtime_error("Expected 1 error message, got " + StringUtils::to_string(g_mockGlobals_ErrorMsgs.size())
			 + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0].length() != expectedError.length())
			throw std::runtime_error("Expected message length is not the same, got: " + StringUtils::to_string(g_mockGlobals_ErrorMsgs[0].length()) +
			", expected: " + StringUtils::to_string(expectedError.length()) + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0] != expectedError)
			throw std::runtime_error("Expected error message not found in logs:\ngot:\n" 
			+ g_mockGlobals_ErrorMsgs[0] + '\n' + "expected :\n" + expectedError + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));



		std::cout << "	PASSED (using a non-existant interpreter)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

/*************************************************************** */
	// Test passing a good interpreter to execve, but a bad script leading to failure
	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::string		testFailure;

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(10, 100, 5000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");
		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);

		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_RUNTIME, &TestProtoRequest_CgiGateway::onErrorRuntime);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_STARTUP, &TestProtoRequest_CgiGateway::onErrorStartup);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_TIMEOUT, &TestProtoRequest_CgiGateway::onErrorTimeOut);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("asfafasfasfasfasf");

		
		protoRequest.m_CgiRequestData->setEnvBase(CgiEnvEnum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000); //0.2ms
		


		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		/// setting up some fds to divert python3 error messages for "no such file or directory"
		int testpipe[2];
		int stdcerrDup = dup(STDERR_FILENO);
		pipe(testpipe);
		dup2(testpipe[1], STDERR_FILENO);
		FileDescriptor
::setNonBlocking(testpipe[0]);
		char pipeDrain[1024];
		/////////////////

		cgi.enqueueRequest(*protoRequest.m_CgiRequestData, false);


		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getSubscribeCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
			while (read(testpipe[0], pipeDrain, sizeof(pipeDrain)) > 0);
		}
			

		

		// tests
		if (eventManager.getSubscribeCount() != 0)
			testFailure = testFailure + '\n' + "EventManager still has events, got " + StringUtils::to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);	

		if (cgi.getBusyWorkerCount() != 0)
			testFailure = testFailure + '\n' + "Cgi::Module still has workers rolling, got " + StringUtils::to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);
		
		if (protoRequest.m_CgiResultStatus != TestProtoRequest::E_CGI_STATUS_ERROR_RUNTIME)
			testFailure = testFailure + '\n' + "Cgi::Module did not cancel the request " + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

		

		// restoring the original stdcerr not to mess the remaining tests
		dup2(stdcerrDup, STDERR_FILENO);
		close(stdcerrDup);
		close(testpipe[1]); 
		close(testpipe[0]);
		/////////////////////////////

		if (!testFailure.empty())
			throw std::runtime_error(testFailure);
		std::cout << "	PASSED (valid interpreter, bad script)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();
/*************************************************************** */

// passing an extension that is not registered
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		Cgi::Module cgi(10, 100, 1000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_RUNTIME, &TestProtoRequest_CgiGateway::onErrorRuntime);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_STARTUP, &TestProtoRequest_CgiGateway::onErrorStartup);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_ERROR_TIMEOUT, &TestProtoRequest_CgiGateway::onErrorTimeOut);
		protoRequest.m_CgiRequestData->setRuntime_Callback(CgiRuntime_Callback::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(CgiIO_Callback::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

		
		protoRequest.m_CgiRequestData->setEnvBase(CgiEnvEnum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000); //0.2ms
		

		
		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		cgi.enqueueRequest(*protoRequest.m_CgiRequestData, false);

		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getSubscribeCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
		}



		// tests
		if (eventManager.getSubscribeCount() != 0)
			throw std::runtime_error("EventManager still has events, got " + StringUtils::to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));	

		if (cgi.getBusyWorkerCount() != 0)
			throw std::runtime_error("Cgi::Module still has workers rolling, got " + StringUtils::to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::string expectedError("InternalCgiWorker::mf_prepareExecve(): interpreter not found");

		if (g_mockGlobals_ErrorMsgs.size() != 1)
			throw std::runtime_error("Expected 1 error message, got " + StringUtils::to_string(g_mockGlobals_ErrorMsgs.size())
			 + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0].length() != expectedError.length())
			throw std::runtime_error("Expected message length is not the same, got: " + StringUtils::to_string(g_mockGlobals_ErrorMsgs[0].length()) +
			", expected: " + StringUtils::to_string(expectedError.length()) + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0] != expectedError)
			throw std::runtime_error("Expected error message not found in logs:\ngot:\n" 
			+ g_mockGlobals_ErrorMsgs[0] + '\n' + "expected :\n" + expectedError + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (demanding an interpreter extension that is not set)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();


	return (testNumber);
}