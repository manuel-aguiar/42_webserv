/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:47:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/09 11:42:55 by mmaria-d         ###   ########.fr       */
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


		while (1)
		{
			eventManager.ProcessEvents(1000);
			if (eventManager.getSubscribeCount() == 0)
				break;
		}

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

	return (testNumber);
}