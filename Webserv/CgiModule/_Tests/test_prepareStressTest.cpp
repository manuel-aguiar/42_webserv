/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_prepareStressTest.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 12:29:30 by mmaria-d         ###   ########.fr       */
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

extern void prepareExpectedOutput(bool isExpectedValid, A_ProtoRequest& proto);

extern std::vector<std::string> g_mockGlobals_ErrorMsgs;

// Tests about the resilience of the CgiModule under heavy load

int Impl_StressTest(int testNumber, const int workers, const int backlog, const int connectionCount)
{
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);

		size_t acquireCounter = 0;

		CgiModule cgi(workers, backlog, globals);

		cgi.addInterpreter("py", "/usr/bin/python3");
		cgi.addInterpreter("sh", "/usr/bin/bash");
		cgi.addInterpreter("php", "/usr/bin/php");

		DynArray<A_ProtoRequest> requests;
		requests.reserve(connectionCount);

		for (int i = 0; i < connectionCount; ++i)
		{
			requests.emplace_back(eventManager, globals, cgi, i);

			requests.back().m_CgiRequestData = cgi.acquireRequestData();
			
			if (requests.back().m_CgiRequestData == NULL) continue;
			
			acquireCounter++;
			requests.back().m_CgiRequestData->setEventManager(eventManager);

			// setup callbacks and environment variables
			for (size_t j = 0; j < E_CGI_CALLBACK_COUNT; j++)
				requests.back().m_CgiRequestData->setCallback(static_cast<e_CgiCallback>(j), &requests[i], A_ProtoRequest_CgiGateway::Callbacks[j]);
			switch (i % 3)
			{
				case 0:
					requests.back().m_CgiRequestData->setExtension("py");
					requests.back().m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
					break;
				case 1:
					requests.back().m_CgiRequestData->setExtension("sh");
					requests.back().m_CgiRequestData->setScriptPath("TestScripts/sh/envPrint.sh");
					break;
				case 2:
					requests.back().m_CgiRequestData->setExtension("php");
					requests.back().m_CgiRequestData->setScriptPath("TestScripts/php/envPrint.php");
					break;
			}

			prepareExpectedOutput(true, requests.back());

			cgi.executeRequest(*requests.back().m_CgiRequestData);
			
			// process events right now at each loop, that way we make room in the CgiModule
			// to take more clients
			eventManager.ProcessEvents(1000);
		}


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

		bool test = true;
		for (int i = 0; i < connectionCount; ++i)
		{
			if (!requests[i].m_CgiRequestData)
				continue;
			if (requests[i].m_TotalBytesRead != requests[i].m_ExpectedOutput.length() ||
				std::string(requests[i].m_buffer) != requests[i].m_ExpectedOutput)
			{
				std::cout << i << " failed: " << requests[i].m_TotalBytesRead << " " << requests[i].m_buffer << "\n\n";
				std::cout << "original: " << requests[i].m_ExpectedOutput.length() << " " << requests[i].m_ExpectedOutput << "\n\n";
				test = false;
			}
		}
		if (!test)
			throw std::logic_error("script output does not match expected output, cases above");


			
		std::cout << "	PASSED [Stress Testing] served: " << acquireCounter << " out of " << connectionCount
		<< " with " << workers << " workers and " << backlog << " backlog" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	// clear the error messages not to mess with the remaining tests
	g_mockGlobals_ErrorMsgs.clear();


	return (testNumber);
}