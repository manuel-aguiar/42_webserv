

# include "../CgiModule/CgiModule.hpp"
# include "../../Globals/Globals.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"

# include "TestDependencies.hpp"
# include "TestProtoConnections/A_ProtoRequest.hpp"
# include <iostream>

/************************************ */
/*	Test using the actual EventManager*/	
/************************************ */

#include <unistd.h>

int main(void)
{
	std::cout << "Test1: ";
	try
	{
		Globals globals(NULL, NULL, NULL, NULL);
		EventManager eventManager(globals);
		CgiModule cgi(10, 100, globals);
		A_ProtoRequest request(eventManager, globals, cgi);

		cgi.addInterpreter("py", "/usr/bin/python3");
		cgi.addInterpreter("sh", "/usr/bin/bash");
		cgi.addInterpreter("php", "/usr/bin/php");

		request.requestCgi();

		while (cgi.getBusyWorkerCount() > 0)
		{
			size_t waited = eventManager.retrieveEvents(1000);
			(void)waited;
			//std::cout << " triggered events: " <<  waited << ", liverequests: " << cgi.getBusyWorkerCount() << " \n";
		}

		if (eventManager.getSubscribeCount() != 0)
			throw std::logic_error("eventManager still has events subscribed");

		std::cout << "	PASS\n";

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	



	return (0);
}

