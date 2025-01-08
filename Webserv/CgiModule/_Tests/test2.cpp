/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:46:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 15:58:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../CgiModule/CgiModule.hpp"

// test helpers
# include "TestProtoConnections/A_ProtoRequest.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"

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


int TestPart2(int testNumber)
{
	std::cout << "Test1: ";
	try
	{
		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, globals);
		A_ProtoRequest protoRequest(eventManager, globals, cgi);

		cgi.addInterpreter("py", "/usr/bin/python3");
		cgi.addInterpreter("sh", "/usr/bin/bash");
		cgi.addInterpreter("php", "/usr/bin/php");

		protoRequest.m_CgiRequestData = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_CALLBACK_COUNT; i++)
			protoRequest.m_CgiRequestData->setCallback(static_cast<e_CgiCallbacks>(i), &protoRequest, A_ProtoRequest_CgiGateway::Callbacks[i]);
		
		protoRequest.m_CgiRequestData->setExtension("py");
		protoRequest.m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
		cgi.executeRequest(*protoRequest.m_CgiRequestData);


		while (cgi.getBusyWorkerCount() > 0)
		{
			size_t waited = eventManager.ProcessEvents(1000);
			(void)waited;
			//std::cout << " triggered events: " <<  waited << ", liverequests: " << cgi.getBusyWorkerCount() << " \n";
		}

		if (eventManager.getSubscribeCount() != 0)
			throw std::logic_error("eventManager still has events subscribed");

		if (protoRequest.m_TotalBytesRead != ::strlen(scriptOutput) ||
			::strncmp(protoRequest.m_buffer, scriptOutput, protoRequest.m_TotalBytesRead) != 0)
			throw std::logic_error("script output does not match expected output");

		std::cout << "	PASS\n";

	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	std::cout << "Test2: ";
	try
	{
		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		const int connectionCount = 3;

		CgiModule cgi(10, connectionCount, globals);

		cgi.addInterpreter("py", "/usr/bin/python3");
		cgi.addInterpreter("sh", "/usr/bin/bash");
		cgi.addInterpreter("php", "/usr/bin/php");

		DynArray<A_ProtoRequest> requests;
		requests.reserve(connectionCount);

		for (size_t i = 0; i < connectionCount; ++i)
		{
			requests.emplace_back(eventManager, globals, cgi);
			requests[i].m_CgiRequestData = cgi.acquireRequestData();
			for (size_t j = 0; j < E_CGI_CALLBACK_COUNT; j++)
				requests[i].m_CgiRequestData->setCallback(static_cast<e_CgiCallbacks>(j), &requests[i], A_ProtoRequest_CgiGateway::Callbacks[j]);
			switch (i % 3)
			{
				case 0:
					requests[i].m_CgiRequestData->setExtension("py");
					requests[i].m_CgiRequestData->setScriptPath("TestScripts/py/envPrint.py");
					break;
				case 1:
					requests[i].m_CgiRequestData->setExtension("sh");
					requests[i].m_CgiRequestData->setScriptPath("TestScripts/sh/envPrint.sh");
					break;
				case 2:
					requests[i].m_CgiRequestData->setExtension("php");
					requests[i].m_CgiRequestData->setScriptPath("TestScripts/php/envPrint.php");
					break;
			}
			cgi.executeRequest(*requests[i].m_CgiRequestData);
		}


		while (cgi.getBusyWorkerCount() > 0)
		{
			size_t waited = eventManager.ProcessEvents(1000);
			(void)waited;
			//std::cout << " triggered events: " <<  waited << ", liverequests: " << cgi.getBusyWorkerCount() << " \n";
		}

		if (eventManager.getSubscribeCount() != 0)
			throw std::logic_error("eventManager still has events subscribed");

		bool test = true;
		for (size_t i = 0; i < connectionCount; ++i)
		{
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

		std::cout << "	PASS\n";

	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}	

	return (testNumber);
}