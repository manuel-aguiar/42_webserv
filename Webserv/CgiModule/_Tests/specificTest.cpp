/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specificTest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:47:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/15 16:42:55 by mmaria-d         ###   ########.fr       */
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

int SpecificTest(int testNumber)
{
	// passing an extension that is not registered
try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, 5000, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi, 0);

		cgi.addInterpreter("py", "/usr/bin/python3");

		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setTimeoutMs(5000); // 5ms
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		protoRequest.m_CgiRequestData->setEventManager(eventManager);
		
		protoRequest.m_CgiRequestData->setEnvBase(E_CGI_AUTH_TYPE, "Basic");

		CgiStressTest::prepareExpectedOutput(true, protoRequest);

		cgi.enqueueRequest(*protoRequest.m_CgiRequestData);


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

		std::cout << "	PASSED (executing a script)" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();

	return (testNumber);
}
