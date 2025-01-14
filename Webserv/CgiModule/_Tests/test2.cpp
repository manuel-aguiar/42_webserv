/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:47:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/14 15:12:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../CgiModule/CgiModule.hpp"

# include "CgiStressTest.hpp"

// test helpers
# include "TestProtoConnections/A_ProtoRequest.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/_Tests/test.h"

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

int TestPart2(int testNumber)
{
	// executing a script, no environment variables so far
	// script should run without issue
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, 1000, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);
		
		///////////////// Setting Variables ////////////////////

		// normal ones
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_CONTENT_LENGTH, "123");

		// custom
		protoRequest.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");

		// doubled entry, the first should prevail
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "DoubledBasic");


		////////////////////////////////////////////////////////////////

		CgiStressTest::prepareExpectedOutput(true, protoRequest);

		cgi.executeRequest
(*protoRequest.m_CgiRequestData);

		//event loop
		while (eventManager.getSubscribeCount() != 0)
			eventManager.ProcessEvents(1000);


		// tests
		if (eventManager.getSubscribeCount() != 0)
			throw std::runtime_error("EventManager still has events, got " + to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));	

		if (cgi.getBusyWorkerCount() != 0)
			throw std::runtime_error("CgiModule still has workers rolling, got " + to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (protoRequest.m_TotalBytesRead != protoRequest.m_ExpectedOutput.length() ||
			std::string(protoRequest.m_buffer) != protoRequest.m_ExpectedOutput)
			throw std::logic_error("Script output doesn't match expected\n\ngot:\n\n" + std::string(protoRequest.m_buffer) + "\nexpected:\n\n" 
			+ protoRequest.m_ExpectedOutput + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (env vars + testing env variables)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();


/*************************************************************** */

	// Test passing a bad interpreter to execve, leading to failure
	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::string		testFailure;
		g_mockGlobals_ErrorMsgs.clear();

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, 1000, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi, 0);

		cgi.addInterpreter("py", "potato");
		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);

		///////////////// Setting Variables ////////////////////

		// normal ones
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_CONTENT_LENGTH, "123");

		// custom
		protoRequest.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");

		// doubled entry, the first should prevail
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "DoubledBasic");


		////////////////////////////////////////////////////////////////

		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		cgi.executeRequest
(*protoRequest.m_CgiRequestData);

		//event loop
		while (eventManager.getSubscribeCount() != 0)
			eventManager.ProcessEvents(1000);


		// tests
		if (eventManager.getSubscribeCount() != 0)
			throw std::runtime_error("EventManager still has events, got " + to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (cgi.getBusyWorkerCount() != 0)
			throw std::runtime_error("CgiModule still has workers rolling, got " + to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::string expectedError("InternalCgiWorker::mf_executeChild(), execve(): No such file or directory");

		if (g_mockGlobals_ErrorMsgs.size() != 1)
			throw std::runtime_error("Expected 1 error message, got " + to_string(g_mockGlobals_ErrorMsgs.size())
			 + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0].length() != expectedError.length())
			throw std::runtime_error("Expected message length is not the same, got: " + to_string(g_mockGlobals_ErrorMsgs[0].length()) +
			", expected: " + to_string(expectedError.length()) + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0] != expectedError)
			throw std::runtime_error("Expected error message not found in logs:\ngot:\n" 
			+ g_mockGlobals_ErrorMsgs[0] + '\n' + "expected :\n" + expectedError + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));



		std::cout << "	PASSED (env vars + using a non-existant interpreter)" << std::endl;
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
		CgiModule cgi(10, 100, 1000, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");
		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("asgasgasgasgasg");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);

		///////////////// Setting Variables ////////////////////

		// normal ones
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_CONTENT_LENGTH, "123");

		// custom
		protoRequest.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");

		// doubled entry, the first should prevail
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "DoubledBasic");


		////////////////////////////////////////////////////////////////


		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		/// setting up some fds to divert python3 error messages for "no such file or directory"
		int testpipe[2];
		int stdcerrDup = dup(STDERR_FILENO);
		pipe(testpipe);
		dup2(testpipe[1], STDERR_FILENO);
		FileDescriptor::setNonBlocking(testpipe[0]);
		char pipeDrain[1024];
		/////////////////

		cgi.executeRequest(*protoRequest.m_CgiRequestData);

		//event loop
		while (eventManager.getSubscribeCount() != 0)
		{
			eventManager.ProcessEvents(1000);

			// pipedrain
			while (read(testpipe[0], pipeDrain, sizeof(pipeDrain)) > 0);
		}


		// tests
		if (eventManager.getSubscribeCount() != 0)
			testFailure = testFailure + '\n' + "EventManager still has events, got " + to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);	

		if (cgi.getBusyWorkerCount() != 0)
			testFailure = testFailure + '\n' + "CgiModule still has workers rolling, got " + to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);
		
		if (protoRequest.m_CgiResultStatus != A_ProtoRequest::E_CGI_STATUS_ERROR_RUNTIME)
			testFailure = testFailure + '\n' + "CgiModule did not cancel the request " + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);


		// restoring the original stdcerr not to mess the remaining tests
		dup2(stdcerrDup, STDERR_FILENO);
		close(stdcerrDup);
		close(testpipe[1]); 
		close(testpipe[0]);
		/////////////////////////////

		if (!testFailure.empty())
			throw std::runtime_error(testFailure);
		std::cout << "	PASSED (env vars + valid interpreter, bad script)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();
/*************************************************************** */
/****************************************************** */

	// passing an extension that is not registered
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, 1000, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi, 0);

		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);

		///////////////// Setting Variables ////////////////////

		// normal ones
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "Basic");
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_CONTENT_LENGTH, "123");

		// custom
		protoRequest.m_CgiRequestData->setEnvExtra("CUSTOM_ENTRY2", "someRandomValue");

		// doubled entry, the first should prevail
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "DoubledBasic");


		////////////////////////////////////////////////////////////////

		CgiStressTest::prepareExpectedOutput(false, protoRequest);

		cgi.executeRequest
(*protoRequest.m_CgiRequestData);

		cgi.finishTimedOut();
		//event loop
		while (eventManager.getSubscribeCount() != 0)
			eventManager.ProcessEvents(1000);


		// tests
		if (eventManager.getSubscribeCount() != 0)
			throw std::runtime_error("EventManager still has events, got " + to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));	

		if (cgi.getBusyWorkerCount() != 0)
			throw std::runtime_error("CgiModule still has workers rolling, got " + to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::string expectedError("InternalCgiWorker::mf_prepareExecve(): interpreter not found");

		if (g_mockGlobals_ErrorMsgs.size() != 1)
			throw std::runtime_error("Expected 1 error message, got " + to_string(g_mockGlobals_ErrorMsgs.size())
			 + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0].length() != expectedError.length())
			throw std::runtime_error("Expected message length is not the same, got: " + to_string(g_mockGlobals_ErrorMsgs[0].length()) +
			", expected: " + to_string(expectedError.length()) + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (g_mockGlobals_ErrorMsgs[0] != expectedError)
			throw std::runtime_error("Expected error message not found in logs:\ngot:\n" 
			+ g_mockGlobals_ErrorMsgs[0] + '\n' + "expected :\n" + expectedError + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED (env vars + demanding an interpreter extension that is not set)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

	return (testNumber);
}