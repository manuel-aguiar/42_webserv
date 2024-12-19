

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
	std::cout << "Test1: ";
	// just a spawn test
	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiModule 		cgi(10, 100, globals);

		//CgiRequestData requestData2;  // correctly doesn't allow compilation, protected constructors
		//InternalCgiWorker liveRequest;	// correctly doesn't allow compilation, private to CgiModule

		(void)cgi;

		std::cout << "		PASSED\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

/******************************************/
/******************************************/
/******************************************/
/******************************************/

	std::cout << "Test2: ";
	// acquiring data
	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiModule 		cgi(10, 100, globals);

		//CgiRequestData requestData2;  // correctly doesn't allow compilation, protected constructors
		//InternalCgiWorker liveRequest;	// correctly doesn't allow compilation, private to CgiModule

		CgiRequestData* requestData1 = cgi.acquireRequestData();
		CgiRequestData* requestData2 = cgi.acquireRequestData();
		CgiRequestData* requestData3 = cgi.acquireRequestData();
		CgiRequestData* requestData4 = cgi.acquireRequestData();


		(void)requestData1;
		(void)requestData2;
		(void)requestData3;
		(void)requestData4;

		std::cout << "		PASSED\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}


/******************************************/
/******************************************/
/******************************************/
/******************************************/

	std::cout << "Test3: ";
	// canceling a request
	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiModule 		cgi(10, 100, globals);

		//CgiRequestData requestData2;  // correctly doesn't allow compilation, protected constructors
		//InternalCgiWorker liveRequest;	// correctly doesn't allow compilation, private to CgiModule

		CgiRequestData* requestData1 = cgi.acquireRequestData();
		CgiRequestData* requestData2 = cgi.acquireRequestData();
		CgiRequestData* requestData3 = cgi.acquireRequestData();
		CgiRequestData* requestData4 = cgi.acquireRequestData();

		cgi.cancelRequest(*requestData3);

		(void)requestData1;
		(void)requestData2;
		(void)requestData3;
		(void)requestData4;

		std::cout << "		PASSED\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

/******************************************/
/******************************************/
/******************************************/
/******************************************/

	std::cout << "Test4: ";
	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiUser 		user;
		EventManager 	eventManager(globals);
		CgiModule 		cgi(10, 100, globals);

		//CgiRequestData requestData2;  // correctly doesn't allow compilation, protected constructors
		//InternalCgiWorker liveRequest;	// correctly doesn't allow compilation, private to CgiModule

		cgi.addInterpreter("py", "/usr/bin/python3");

		
		CgiRequestData* requestData1 = cgi.acquireRequestData();

		//subscribe event callbacks
		for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
			requestData1->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
		
		requestData1->setMsgBody("Hello World!");
		requestData1->setExtension("py");
		requestData1->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
		requestData1->setEventManager(eventManager);

/******************************************/

		cgi.executeRequest(*requestData1);

		while (cgi.getBusyWorkerCount() > 0)
		{
			size_t waited = eventManager.retrieveEvents(1000);
			(void)waited;
			//std::cout << " triggered events: " <<  waited << ", liverequests: " << cgi.getBusyWorkerCount() << " \n";
		}

		if (eventManager.getSubscribeCount() != 0)
			throw std::logic_error("eventManager still has events subscribed");

		std::cout << "		PASSED\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

/******************************************/
/******************************************/
/******************************************/
/******************************************/	


	std::cout << "Test5: ";
	// multiple requests executing, there are executors available for everyone
	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiUser 		user;
		EventManager 	eventManager(globals);
		CgiModule 		cgi(10, 100, globals);							//10 executors available

		cgi.addInterpreter("py", "/usr/bin/python3");
		CgiRequestData* requestData1 = cgi.acquireRequestData();

		for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
			requestData1->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
		requestData1->setMsgBody("Hello World!");
		requestData1->setExtension("py");
		requestData1->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
		requestData1->setEventManager(eventManager);


		CgiRequestData* requestData2 = cgi.acquireRequestData();

		//subscribe event callbacks
		for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
			requestData2->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
		
		requestData2->setMsgBody("Hello World!");
		requestData2->setExtension("py");
		requestData2->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
		requestData2->setEventManager(eventManager);


		CgiRequestData* requestData3 = cgi.acquireRequestData();

		//subscribe event callbacks
		for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
			requestData3->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
		
		requestData3->setMsgBody("Hello World!");
		requestData3->setExtension("py");
		requestData3->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
		requestData3->setEventManager(eventManager);


		cgi.executeRequest(*requestData1);
		cgi.executeRequest(*requestData2);
		cgi.executeRequest(*requestData3);

		while (cgi.getBusyWorkerCount() > 0)
		{
			size_t waited = eventManager.retrieveEvents(1000);
			(void)waited;
			//std::cout << " triggered events: " <<  waited << ", liverequests: " << cgi.getBusyWorkerCount() << " \n";
		}

		if (eventManager.getSubscribeCount() != 0)
			throw std::logic_error("eventManager still has events subscribed");

		std::cout << "		PASSED\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

/******************************************/
/******************************************/
/******************************************/
/******************************************/

	std::cout << "Test6: ";
	// multiple requests executing, no executors for everyone, have to queue
	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiUser 		user;
		EventManager 	eventManager(globals);
		CgiModule 		cgi(1, 5, globals);							//<- only 1 executor

		cgi.addInterpreter("py", "/usr/bin/python3");
		
			CgiRequestData* requestData1 = cgi.acquireRequestData();

			for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
				requestData1->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
			requestData1->setMsgBody("Hello World!");
			requestData1->setExtension("py");
			requestData1->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
			requestData1->setEventManager(eventManager);


		CgiRequestData* requestData2 = cgi.acquireRequestData();

		//subscribe event callbacks
		for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
			requestData2->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
		
		requestData2->setMsgBody("Hello World!");
		requestData2->setExtension("py");
		requestData2->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
		requestData2->setEventManager(eventManager);


		CgiRequestData* requestData3 = cgi.acquireRequestData();
		//subscribe event callbacks
		for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
			requestData3->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
		
		requestData3->setMsgBody("Hello World!");
		requestData3->setExtension("py");
		requestData3->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
		requestData3->setEventManager(eventManager);

		cgi.executeRequest(*requestData1);
		cgi.executeRequest(*requestData2);
		cgi.executeRequest(*requestData3);

		while (cgi.getBusyWorkerCount() > 0)
		{
			size_t waited = eventManager.retrieveEvents(1000);
			(void)waited;
			//std::cout << " triggered events: " <<  waited << ", liverequests: " << cgi.getBusyWorkerCount() << " \n";
		}
		if (eventManager.getSubscribeCount() != 0)
			throw std::logic_error("eventManager still has events subscribed");
		std::cout << "		PASSED\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

/******************************************/
/******************************************/
/******************************************/
/******************************************/

	std::cout << "Test7: ";
	// Just abusing this
	try
	{
		//setup
		Globals 		globals(NULL, NULL, NULL, NULL);
		CgiUser 		user;
		EventManager 	eventManager(globals);
		CgiModule 		cgi(3, 1000, globals);							//<- only 10 workers, 1000 backlog

		cgi.addInterpreter("py", "/usr/bin/python3");
		
		// failing, potential pipe not beign correctly reset

		for (size_t i = 0; i < 1000; i++)
		{
			CgiRequestData* requestData1 = cgi.acquireRequestData();

			for (size_t i = 0; i < E_CGI_EVENT_COUNT; i++)
				requestData1->setEventHandler(static_cast<e_CgiEvents>(i), &user, CgiUserGateway::eventHandlers[i]);
			requestData1->setMsgBody("Hello World!");
			requestData1->setExtension("py");
			requestData1->setScriptPath("../../../Testing/MockWebsites/Website1/cgi-bin/hello.py");
			requestData1->setEventManager(eventManager);
			cgi.executeRequest(*requestData1);
		}

		while (cgi.getBusyWorkerCount() > 0)
		{
			size_t waited = eventManager.retrieveEvents(1000);
			(void)waited;
			//std::cout << " triggered events: " <<  waited << ", liverequests: " << cgi.getBusyWorkerCount() << std::endl;
		}
		if (eventManager.getSubscribeCount() != 0)
			throw std::logic_error("eventManager still has events subscribed");
		std::cout << "		PASSED\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}



	return (0);
}

