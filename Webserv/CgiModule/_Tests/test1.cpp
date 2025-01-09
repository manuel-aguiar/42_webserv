/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:47:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/09 12:46:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../CgiModule/CgiModule.hpp"

// test helpers
# include "TestProtoConnections/A_ProtoRequest.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../../Toolkit/_Tests/test.h"

//C++ headers
# include <iostream>

// C headers
#include <unistd.h>

static const char* scriptOutput = "AUTH_TYPE: <not set>\n"
"CONTENT_LENGTH: <not set>\n"
"CONTENT_TYPE: <not set>\n"
"GATEWAY_INTERFACE: <not set>\n"
"PATH_INFO: <not set>\n"
"PATH_TRANSLATED: <not set>\n"
"QUERY_STRING: <not set>\n"
"REMOTE_ADDR: <not set>\n"
"REMOTE_HOST: <not set>\n"
"REMOTE_IDENT: <not set>\n"
"REMOTE_USER: <not set>\n"
"REQUEST_METHOD: <not set>\n"
"SCRIPT_NAME: <not set>\n"
"SERVER_NAME: <not set>\n"
"SERVER_PORT: <not set>\n"
"SERVER_PROTOCOL: <not set>\n"
"SERVER_SOFTWARE: <not set>\n";

int TestPart1(int testNumber)
{
	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		Globals globals(NULL, NULL, NULL, NULL);
		CgiModule cgi(10, 100, globals);				// 10 workers, 100 backlog

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);
		
		cgi.executeRequest(*protoRequest.m_CgiRequestData);

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

		if (protoRequest.m_TotalBytesRead != ::strlen(scriptOutput) ||
			::strncmp(protoRequest.m_buffer, scriptOutput, protoRequest.m_TotalBytesRead) != 0)
			throw std::logic_error("Script output doesn't match expected\n\ngot:\n\n" + std::string(protoRequest.m_buffer) + "\nexpected:\n\n" 
			+ scriptOutput + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

/*************************************************************** */

	// Test passing a bad interpreter to execve, leading to failure
	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::string		testFailure;

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi);

		cgi.addInterpreter("py", "potato");
		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);

		// setting up some fds to read the logs
		int testpipe[2];
		int stdcerrDup = dup(STDERR_FILENO);
		pipe(testpipe);
		dup2(testpipe[1], STDERR_FILENO);



		cgi.executeRequest(*protoRequest.m_CgiRequestData);

		//event loop
		while (eventManager.getSubscribeCount() != 0)
			eventManager.ProcessEvents(1000);


		// tests
		if (eventManager.getSubscribeCount() != 0)
			testFailure = testFailure + '\n' + "EventManager still has events, got " + to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

		if (cgi.getBusyWorkerCount() != 0)
			testFailure = testFailure + '\n' + "CgiModule still has workers rolling, got " + to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

		// read actual stdcerr
		char buffer[1024];
		int bytesRead = read(testpipe[0], buffer, 1023);
		buffer[bytesRead] = 0;
		std::string expectedError("InternalCgiWorker::mf_readEmergencyPipe(), execve(): No such file or directory\n");

		if (std::strlen(buffer) != expectedError.length())
		{
			testFailure = testFailure + '\n' + "Expected message length is not the same, got: " + to_string(std::strlen(buffer)) +
			", expected: " + to_string(expectedError.length()) + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);
		}

		if (std::string(buffer) != std::string(expectedError))
			testFailure = testFailure + '\n' + "Expected error message not found in logs:\ngot:\n" 
			+ buffer + '\n' + "expected :\n" + expectedError + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

			
		// restoring the original stdcerr not to mess the remaining tests
		dup2(stdcerrDup, STDERR_FILENO);
		close(stdcerrDup);
		close(testpipe[1]); 
		close(testpipe[0]); 

		if (!testFailure.empty())
			throw std::runtime_error(testFailure);
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
/*************************************************************** */

	// Test passing a bad interpreter to execve, leading to failure
	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::string		testFailure;

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi);

		cgi.addInterpreter("py", "potato");
		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);

		// setting up some fds to read the logs
		int testpipe[2];
		int stdcerrDup = dup(STDERR_FILENO);
		pipe(testpipe);
		dup2(testpipe[1], STDERR_FILENO);



		cgi.executeRequest(*protoRequest.m_CgiRequestData);

		//event loop
		while (eventManager.getSubscribeCount() != 0)
			eventManager.ProcessEvents(1000);


		// tests
		if (eventManager.getSubscribeCount() != 0)
			testFailure = testFailure + '\n' + "EventManager still has events, got " + to_string(eventManager.getSubscribeCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

		if (cgi.getBusyWorkerCount() != 0)
			testFailure = testFailure + '\n' + "CgiModule still has workers rolling, got " + to_string(cgi.getBusyWorkerCount())
			 + " expected 0" + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

		// read actual stdcerr
		char buffer[1024];
		int bytesRead = read(testpipe[0], buffer, 1023);
		buffer[bytesRead] = 0;
		std::string expectedError("InternalCgiWorker::mf_readEmergencyPipe(), execve(): No such file or directory\n");

		if (std::strlen(buffer) != expectedError.length())
		{
			testFailure = testFailure + '\n' + "Expected message length is not the same, got: " + to_string(std::strlen(buffer)) +
			", expected: " + to_string(expectedError.length()) + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);
		}

		if (std::string(buffer) != std::string(expectedError))
			testFailure = testFailure + '\n' + "Expected error message not found in logs:\ngot:\n" 
			+ buffer + '\n' + "expected :\n" + expectedError + '\n' + FileLineFunction(__FILE__, __LINE__, __FUNCTION__);

			
		// restoring the original stdcerr not to mess the remaining tests
		dup2(stdcerrDup, STDERR_FILENO);
		close(stdcerrDup);
		close(testpipe[1]); 
		close(testpipe[0]); 

		if (!testFailure.empty())
			throw std::runtime_error(testFailure);
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}