/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:47:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 18:17:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../CgiModule/CgiModule.hpp"

# include "CgiStressTest.hpp"

// test helpers
# include "TestProtoConnections/A_ProtoRequest.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
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
		CgiModule cgi(10, 100, 5000, eventManager, globals);
		A_ProtoRequest protoRequest(globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		protoRequest.m_CgiRequestData->setUser(&protoRequest);
		protoRequest.m_CgiRequestData->setHandler(E_CGI_ON_ERROR_RUNTIME, &A_ProtoRequest_CgiGateway::onErrorRuntime);
		protoRequest.m_CgiRequestData->setHandler(E_CGI_ON_ERROR_STARTUP, &A_ProtoRequest_CgiGateway::onErrorStartup);
		protoRequest.m_CgiRequestData->setHandler(E_CGI_ON_ERROR_TIMEOUT, &A_ProtoRequest_CgiGateway::onErrorTimeOut);
		protoRequest.m_CgiRequestData->setReadHandler(&A_ProtoRequest_CgiGateway::onRead);
		protoRequest.m_CgiRequestData->setWriteHandler(&A_ProtoRequest_CgiGateway::onWrite);

		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setTimeoutMs(5000);
		

		
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

	return (testNumber);
}