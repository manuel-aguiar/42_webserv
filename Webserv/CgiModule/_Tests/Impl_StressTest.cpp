/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Impl_StressTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 10:10:12 by mmaria-d         ###   ########.fr       */
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
			if (requests[i].m_TotalBytesRead != ::strlen(scriptOutput) ||
				::strncmp(requests[i].m_buffer, scriptOutput, requests[i].m_TotalBytesRead) != 0)
			{
				std::cout << i << " failed: " << requests[i].m_TotalBytesRead << " " << requests[i].m_buffer << "\n\n";
				std::cout << "original: " << ::strlen(scriptOutput) << " " << scriptOutput << "\n\n";
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