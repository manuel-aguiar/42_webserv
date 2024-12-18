

# include "../CgiModule/CgiModule.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"

# include "TestDependencies.hpp"
# include <iostream>

/************************************ */
/*	Test using the actual EventManager*/	
/************************************ */

#include <unistd.h>

int main(void)
{

	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiUser 		user;
		EventManager 	eventManager(globals);
		CgiModule 		cgi(10, globals);

		cgi.addInterpreter("py", "/usr/bin/python3");

		CgiRequestData& requestData = cgi.acquireRequestData();

		//subscribe event callbacks
		for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
			requestData.setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
		
		requestData.setMsgBody("Hello World!");
		requestData.setExtension("py");
		requestData.setScriptPath("/workspaces/42_webserv/Testing/MockWebsites/Website1/cgi-bin/hello.py");
		requestData.setEventManager(eventManager);

		cgi.executeRequest(requestData);
		std::cout << " triggered: " <<  eventManager.retrieveEvents(1000) << " \n";
		std::cout << " triggered: " <<  eventManager.retrieveEvents(1000) << " \n";
		std::cout << " triggered: " <<  eventManager.retrieveEvents(1000) << " \n";
		std::cout << " triggered: " <<  eventManager.retrieveEvents(1000) << " \n";
		std::cout << " triggered: " <<  eventManager.retrieveEvents(1000) << " \n";
		std::cout << " triggered: " <<  eventManager.retrieveEvents(1000) << " \n";

		std::cout << "		OK\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	return (0);
}

