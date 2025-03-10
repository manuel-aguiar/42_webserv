

// Project headers
# include "../Globals/Globals.hpp"
# include "../Globals/Clock/Clock.hpp"
# include "../Globals/LogFile/LogFile.hpp"
# include "../SignalHandler/SignalHandler.hpp"
# include "../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../ServerConfig/BlockFinder/BlockFinder.hpp"
# include "../ServerConfig/DefaultConfig/DefaultConfig.hpp"
# include "../ServerContext/ServerContext.hpp"
# include "../CgiModule/Module/Module.hpp"
# include "../HttpModule/HttpModule/HttpModule.hpp"
# include "../Events/Subscription/Subscription.hpp"
# include "../Events/Manager/Manager.hpp"
# include "../Connections/Manager/Manager.hpp"

// C++ headers
# include <iostream>
# include <cstdlib>
# include <cstdio> // EXIT macros

// C headers
# include <signal.h>

static int maxEventsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections();			// max connections
	res += config.getAllBindAddresses().size(); // listening sockets
	res += config.getMaxConcurrentCgi() * 3;  	// read + write + emergency, 1 event each
	res += 1;									// signal handler event, monitor reception of signals	

	return (res);
}

static int maxFdsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections() * 2;			// connections sockets + static files
	res += config.getMaxConcurrentCgi() * 6; 		// read + write + emergency, both pipe ends
	res += config.getAllBindAddresses().size(); 	// listening sockets
	res += 2;										// signal handler pipe
	res *= config.getMaxWorkers();					// workers

	return (res);
}

static size_t blockFinderEntryCount(const ServerConfig& config)
{
	size_t res = 0;
	const std::vector<ServerBlock>& blocks = config.getServerBlocks();

	for (size_t i = 0; i < blocks.size(); ++i)
	{
		res += blocks[i].getListenAddresses().size() * blocks[i].getServerNames().size();
	}

	res += config.getAllBindAddresses().size(); // default block

	return (res);
}

struct RunCheck
{
	RunCheck() : yes(true) {}

	static void Callback(Events::Subscription& event)
	{
		RunCheck* check = static_cast<RunCheck*>(event.accessUser());
		check->yes = false;
	}

	bool yes;
};

static int selectTimeout(const int first, const int second)
{
	if (first == -1)
		return (second);
	if (second == -1)
		return (first);
	return (std::min(first, second));
}

int SingleServerRun(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber)
{
	////////////////////////////////////
	////////// Single Server ///////////
	////////////////////////////////////

	try
	{
		// our number 10
		Events::Manager	eventManager(maxEventsEstimate(config), globals, maxFdsEstimate(config));

		// preparing ServerContext;
		ServerContext	context;
		context.setServerConfig(config);

		BlockFinder		blockFinder(blockFinderEntryCount(config));
		context.setBlockFinder(blockFinder);

		Cgi::Module		cgiModule(config.getMaxConcurrentCgi(), config.getMaxCgiBacklog(), 5000, 4096, eventManager, globals);
		context.setAddonLayer(Ws::AddonLayer::CGI, &cgiModule);		
		
		Http::Module	httpModule(config.getMaxConnections(), context);
		context.setAppLayer(Ws::AppLayer::HTTP, &httpModule, &Http::Module::InitConnection);

		blockFinder.loadServerBlocks(config.getServerBlocks());

		context.setGlobals(globals);

		// preparing server launch
		Conn::Manager	connManager(config.getMaxConnections(), config.getAllBindAddresses(), eventManager, globals, context);

		// monitor signal handler
		RunCheck run;
		Events::Subscription* signalListener = eventManager.acquireSubscription();
		signalListener->setFd(g_SignalHandler.getSignalListener(myServerNumber));
		signalListener->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
		signalListener->setUser(&run);
		signalListener->setCallback(&RunCheck::Callback);
		eventManager.startMonitoring(*signalListener, false);


		// oficially open the listening sockets
		if (!connManager.init())
			throw std::runtime_error("SingleServerRun: connManager.init() failed");

		//////////////////////////////////////////
		/////////// Main Server Loop /////////////
		//////////////////////////////////////////
		while (run.yes)
		{
			int cgiTimeout = cgiModule.processRequests();	
			int httpTimeout = httpModule.closeTimedOutConnections();
			eventManager.ProcessEvents(selectTimeout(cgiTimeout, httpTimeout));
		}
		/////////////////////////////////////////
	}
	catch(const std::exception& e)
	{
		std::cerr << programName << "webserv: CRITICAL FAILURE: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
