

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

void TestPart1(int& testNumber)
{
/******************************************************* */
/******************************************************* */
	//instantiation and cleanup test	
	try
	{
		TEST_INTRO(testNumber++);
		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 1000, 4096, eventManager, globals);				// 10 workers, 100 backlog

		TEST_PASSED_MSG("instantiation and cleanup");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();
/////////////////////////////////////////////////////////////////////////////////////////////////
	//instantiation and cleanup test	
	try
	{
		TEST_INTRO(testNumber++);
		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);

		Cgi::Module cgi(10, 100, 1000, 4096, eventManager, globals);				// 10 workers, 100 backlog
		Cgi::Request* data = cgi.acquireRequest();
		(void)data;
		TEST_PASSED_MSG("acquiring and just going away");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();
//////////////////////////////////////////////////////////////////////////////////////////////////
	//instantiation and cleanup test	
	try
	{
		TEST_INTRO(testNumber++);
		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 1000, 4096, eventManager, globals);				// 10 workers, 100 backlog

		cgi.processRequests();
		TEST_PASSED_MSG("finish timeout with empty queues");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();



/****************************************************** */
/*
// Testing timeout cleanup
	try
	{
		TEST_INTRO(testNumber++);

		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 500, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_ERROR, &TestProtoRequest_CgiGateway::onError);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

		
		
		protoRequest.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
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
			
			if (eventManager.getMonitoringCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
		}


		// tests
		EXPECT_EQUAL(eventManager.getMonitoringCount(), 0, "Manager still has events");
		EXPECT_EQUAL(cgi.getBusyWorkerCount(), 0, "Cgi::Module still has workers rolling");
		EXPECT_EQUAL(protoRequest.m_CgiResultStatus, TestProtoRequest::E_CGI_STATUS_TIMEOUT, "ProtoRequest didn't receive timeout notice");
		EXPECT_EQUAL(protoRequest.m_TotalBytesRead, protoRequest.m_ExpectedOutput.length(), "Script output doesn't match expected");
		EXPECT_EQUAL(std::string(protoRequest.m_buffer), protoRequest.m_ExpectedOutput, "Script output doesn't match expected");

		TEST_PASSED_MSG("finishing requests that timed out");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();


// Test passing a bad interpreter to execve, leading to failure
	
	try
	{
		TEST_INTRO(testNumber++);
		std::string		testFailure;
		g_mockGlobals_ErrorMsgs.clear();

		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 5000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "potato");
		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);

		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_ERROR, &TestProtoRequest_CgiGateway::onError);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

		
		protoRequest.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000); //0.2ms

		CgiStressTest::prepareExpectedOutput(false, protoRequest);

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

		std::string expectedError("InternalCgiWorker::mf_executeChild(), execve(): No such file or directory");

		EXPECT_EQUAL(g_mockGlobals_ErrorMsgs.size(), 1, "Expected 1 error message");
		EXPECT_EQUAL(g_mockGlobals_ErrorMsgs[0].length(), expectedError.length(), "Expected message length is not the same");
		EXPECT_EQUAL(g_mockGlobals_ErrorMsgs[0], expectedError, "Expected error message not found in logs");

		TEST_PASSED_MSG("using a non-existant interpreter");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

///////////////////////////////////////////////////////////////////
	// Test passing a good interpreter to execve, but a bad script leading to failure
	
	try
	{
		TEST_INTRO(testNumber++);
		std::string		testFailure;

		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 5000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");
		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);

		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_ERROR, &TestProtoRequest_CgiGateway::onError);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("asfafasfasfasfasf");

		
		protoRequest.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000); //0.2ms
		


		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		/// setting up some fds to divert python3 error messages for "no such file or directory"
		int testpipe[2];
		int stdcerrDup;
		char pipeDrain[1024];
		EXPECT_EQUAL(pipe(testpipe), 0, "pipe() failed");
		EXPECT_EQUAL((stdcerrDup = dup(STDERR_FILENO)) != -1, true, "dup() failed");
		EXPECT_EQUAL(dup2(testpipe[1], STDERR_FILENO) != -1, true, "dup2() failed");
		EXPECT_EQUAL(FileDescriptor::setNonBlocking(testpipe[0]) == 0, false, "Fcntl failed"); 
		/////////////////

		cgi.enqueueRequest(*protoRequest.m_CgiRequestData, false);


		//event loop
		while (1)
		{
			unsigned int nextWait = cgi.processRequests();
			
			if (eventManager.getMonitoringCount() != 0)
				eventManager.ProcessEvents(nextWait);
			else
				break ;
			while (read(testpipe[0], pipeDrain, sizeof(pipeDrain)) > 0);
		}
			
		// tests
		if (eventManager.getMonitoringCount() != 0)
			testFailure = testFailure + '\n' + TEST_ERROR_MSG(eventManager.getMonitoringCount(), 0, "Manager still has events");

		if (cgi.getBusyWorkerCount() != 0)
			testFailure = testFailure + '\n' + TEST_ERROR_MSG(cgi.getBusyWorkerCount(), 0, "Cgi::Module still has workers rolling");
		
		if (protoRequest.m_CgiResultStatus != TestProtoRequest::E_CGI_STATUS_ERROR)
			testFailure = testFailure + '\n' + TEST_ERROR_MSG(protoRequest.m_CgiResultStatus, TestProtoRequest::E_CGI_STATUS_ERROR, "ProtoRequest didn't receive error runtime notice");
		

		// restoring the original stdcerr not to mess the remaining tests
		dup2(stdcerrDup, STDERR_FILENO);
		close(stdcerrDup);
		close(testpipe[1]); 
		close(testpipe[0]);
		/////////////////////////////

		if (!testFailure.empty())
			throw std::runtime_error(testFailure);
		TEST_PASSED_MSG("valid interpreter, bad script");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();
///////////////////////////////////////////////////////////////////

// passing an extension that is not registered
	try
	{
		TEST_INTRO(testNumber++);

		Globals globals(NULL, NULL, NULL, NULL);
		Events::Manager eventManager(30, globals);
		Cgi::Module cgi(10, 100, 1000, eventManager, globals);
		TestProtoRequest protoRequest(globals, cgi, 0);

		protoRequest.m_CgiRequestData = cgi.acquireRequest();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_ERROR, &TestProtoRequest_CgiGateway::onError);
		protoRequest.m_CgiRequestData->setNotify_Callback(Cgi::Notify::ON_SUCCESS, &TestProtoRequest_CgiGateway::onSuccess);
		
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::READ, &TestProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setIO_Callback(Cgi::IO::WRITE, &TestProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");

		
		protoRequest.m_CgiRequestData->setEnvBase(Cgi::Env::Enum::AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000); //0.2ms
		

		
		CgiStressTest::prepareExpectedOutput(false, protoRequest);

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

		EXPECT_EQUAL(eventManager.getMonitoringCount(), 0, "Manager still has events");
		EXPECT_EQUAL(cgi.getBusyWorkerCount(), 0, "Cgi::Module still has workers rolling");

		std::string expectedError("InternalCgiWorker::mf_prepareExecve(): interpreter not found");

		EXPECT_EQUAL(g_mockGlobals_ErrorMsgs.size(), 1, "Expected 1 error message");
		EXPECT_EQUAL(g_mockGlobals_ErrorMsgs[0].length(), expectedError.length(), "Expected message length is not the same");
		EXPECT_EQUAL(g_mockGlobals_ErrorMsgs[0], expectedError, "Expected error message not found in logs");

		TEST_PASSED_MSG("demanding an interpreter extension that is not set");
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();
*/
}