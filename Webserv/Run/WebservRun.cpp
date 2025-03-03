

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


int	WebservRun(const char* programName, ServerConfig& config);
int SingleServerRun(const char* programName, ServerConfig& config);
int MultiServerRun(const char* programName, ServerConfig& config);

int maxEventsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections();
	res += config.getMaxConcurrentCgi() * 3;  	// read + write + emergency, 1 event each
	res += config.getMaxWorkers();				// signal handler event, monitor reception of signals	

	return (res);
}

int maxFdsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections() * 2;			// connections sockets + static files
	res += config.getMaxConcurrentCgi() * 6; 		// read + write + emergency, both pipe ends
	res += config.getAllBindAddresses().size(); 	// listening sockets
	res += 2;										// signal handler pipe
	res *= config.getMaxWorkers();					// workers

	return (res);
}

size_t blockFinderEntryCount(const ServerConfig& config)
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

int	WebservRun(const char* programName, ServerConfig& config)
{
	if (config.getMaxWorkers() == 1)
		return (SingleServerRun(programName, config));
	
	return (SingleServerRun(programName, config));
	//return (MultiServerRun(programName, config));	//not implemented, run as single server always
}

int SingleServerRun(const char* programName, ServerConfig& config)
{
	////////////////////////////////////
	////////// Single Server ///////////
	////////////////////////////////////

	try
	{
		// starting point and parse config file
		Clock			clock;
		LogFile			statusFile("status.log");
		LogFile			errorFile("error.log");
		LogFile			debugFile("debug.log");
		Globals			globals(&clock, &statusFile, &errorFile, &debugFile);

		// our number 10
		Events::Manager	eventManager(maxEventsEstimate(config), globals, maxFdsEstimate(config));

		// preparing ServerContext;
		ServerContext	context;
		BlockFinder		blockFinder(blockFinderEntryCount(config));

		Cgi::Module		cgiModule(config.getMaxConcurrentCgi(), config.getMaxCgiBacklog(), 5000, eventManager, globals); // NOT ADDED YET
		context.setAddonLayer(Ws::AddonLayer::CGI, &cgiModule);		
		
		// NOT ADDED YET
		Http::Module	httpModule(config.getMaxConnections(), context);


		blockFinder.loadServerBlocks(config.getServerBlocks());

		context.setAppLayer(Ws::AppLayer::HTTP, &httpModule, &Http::Module::InitConnection);
		context.setBlockFinder(blockFinder);
		context.setGlobals(globals);
		context.setServerConfig(config);

		// preparing server launch
		Conn::Manager	connManager(config.getMaxConnections(), config.getAllBindAddresses(), eventManager, globals, context);

		// setup signal handling
		g_SignalHandler.openSignalListeners(1, &globals);
		g_SignalHandler.registerSignal(SIGINT, &globals);
		g_SignalHandler.registerSignal(SIGTERM, &globals);
		g_SignalHandler.registerSignal(SIGQUIT, &globals);

		// monitor signal handler
		RunCheck run;
		Events::Subscription* signalListener = eventManager.acquireSubscription();
		signalListener->setFd(g_SignalHandler.getSignalListener(0));
		signalListener->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
		signalListener->setUser(&run);
		signalListener->setCallback(&RunCheck::Callback);
		eventManager.startMonitoring(*signalListener, false);


		// oficially open the listening sockets
		connManager.init();

		//////////////////////////////////////////
		/////////// Main Server Loop /////////////
		//////////////////////////////////////////
		while (run.yes)
		{
			int cgiTimeout = cgiModule.processRequests();						//NOT ADDED YET		
			int httpTimeout = httpModule.closeTimedOutConnections();
			eventManager.ProcessEvents(std::min(cgiTimeout, httpTimeout));	// NOT ADDED YET
		}
		/////////////////////////////////////////

		// all RAII cleanup
	}
	catch(const std::exception& e)
	{
		std::cerr << programName << "webserv: CRITICAL FAILURE: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	MultiServerRun(const char* programName, const ServerConfig& config)
{
	(void)programName;
	(void)config;
	return (EXIT_SUCCESS);
}
