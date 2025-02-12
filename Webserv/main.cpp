


// Project headers
# include "Globals/Globals.hpp"
# include "Globals/Clock/Clock.hpp"
# include "Globals/LogFile/LogFile.hpp"
# include "SignalHandler/SignalHandler.hpp"
# include "ServerConfig/ServerConfig/ServerConfig.hpp"
# include "ServerConfig/BlockFinder/BlockFinder.hpp"
# include "ServerConfig/DefaultConfig/DefaultConfig.hpp"
# include "ServerContext/ServerContext.hpp"
# include "CgiModule/Module/Module.hpp"
# include "HttpModule/Module/Module.hpp"
# include "Events/Subscription/Subscription.hpp"
# include "Events/Manager/Manager.hpp"
# include "Connections/Manager/Manager.hpp"

// C++ headers
# include <iostream>
# include <cstdlib>

// C headers
# include <signal.h>

int maxEventsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections();
	res += config.getMaxConcurrentCgi() * 3;  	// read + write + emergency, 1 event each
	//res += config.getMaxWorkers();
	res += 1; 									// signal handler event, monitor reception of signals	

	return (res);
}

int maxFdsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += config.getMaxConnections() * 2;			// connections sockets + static files
	res += config.getMaxConcurrentCgi() * 6; 		// read + write + emergency, both pipe ends
	res += config.getAllBindAddresses().size(); 	// listening sockets
	res += 2;										// signal handler pipe

	return (res);
}

SignalHandler g_SignalHandler;

struct RunCheck
{
	RunCheck() : yes(true) {}

	static void RunCheck_Callback(Events::Subscription& event)
	{
		RunCheck* check = static_cast<RunCheck*>(event.accessUser());
		check->yes = false;
	}

	bool yes;
};

int main(int ac, char** av)
{
    if (ac != 2)
	{
		std::cerr << av[0] << ": Usage: " << av[0] << " <config_file>" << std::endl;
		return (EXIT_FAILURE);
	}
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
		DefaultConfig	defaultConfig;
		ServerConfig	config(av[1], defaultConfig, &globals);

		if (!config.parseConfigFile())
			return (EXIT_FAILURE);

		// major dependency
		Events::Manager	eventManager(maxEventsEstimate(config), globals, maxFdsEstimate(config));

		// preparing ServerContext;
		ServerContext	context;
		BlockFinder		blockFinder;
		Http::Module	httpModule(config.getMaxConnections(), defaultConfig, globals);
		Cgi::Module		cgiModule(config.getMaxConcurrentCgi(), config.getMaxCgiBacklog(), 5000, eventManager, globals);

		blockFinder.loadServerBlocks(config.getServerBlocks());

		context.setAppLayer(Ws::AppLayer::HTTP, &httpModule, &Http::Module::InitConnection);
		context.setAddonLayer(Ws::AddonLayer::CGI, &cgiModule);
		context.setBlockFinder(blockFinder);
		context.setGlobals(globals);
		context.setServerConfig(config);

		// preparing server launch
		Conn::Manager	connManager(config.getMaxConnections(), config.getAllBindAddresses(), eventManager, globals, context);

		// setup signal handling
		g_SignalHandler.openSignalListeners(1, globals);
		g_SignalHandler.registerSignal(SIGINT, globals);
		g_SignalHandler.registerSignal(SIGTERM, globals);
		g_SignalHandler.registerSignal(SIGQUIT, globals);

		// monitor signal handler
		RunCheck run;
		Events::Subscription* signalListener = eventManager.acquireSubscription();
		signalListener->setFd(g_SignalHandler.getSignalListener(0));
		signalListener->setMonitoredEvents(Events::Monitor::READ);
		signalListener->setUser(&run);
		signalListener->setCallback(&RunCheck::RunCheck_Callback);
		eventManager.startMonitoring(*signalListener, false);


		// oficially open the listening sockets
		connManager.init();

		//////////////////////////////////////////
		/////////// Main Server Loop /////////////
		//////////////////////////////////////////
		while (run.yes)
		{
			int cgiTimeout = cgiModule.processRequests();
			int httpTimeout = httpModule.closeTimedOutConnections();
			eventManager.ProcessEvents(std::min(cgiTimeout, httpTimeout));
		}
		/////////////////////////////////////////

		// Non-RAII cleanup
		connManager.shutdown();
		eventManager.stopMonitoring(*signalListener, false);
		eventManager.returnSubscription(*signalListener);
	}
	catch(const std::exception& e)
	{
		std::cerr << av[0] << ": CRITICAL FAILURE: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	
    return (EXIT_SUCCESS);
}
