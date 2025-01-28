

# include "Module.hpp"
# include "../Connection/Connection.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Events/Subscription/Subscription.hpp"

void	Http::Module::InitConnection(Conn::BaseConnection& connection)
{
	ServerContext& context = connection.accessServerContext();

	// blabla could do something
	Http::Module* module = reinterpret_cast<Http::Module*>(context.getAppLayerModule(Ws::AppLayer::HTTP));


	Http::Connection* httpConn = new Http::Connection();

	// inform connection about the app layer connection, and the forced close callback
	connection.setAppLayerConn(httpConn);
	connection.setAppLayerForceClose(Http::Module::CallbackForcedClose);
	// let httpConn where its connection is, future access to events, etc
	httpConn->setMyTCP(&connection);

	Events::Subscription* subs = connection.accessEventSubs();

	subs->setUser(httpConn);
	subs->setCallback(Http::Connection::IO_Callback);
	subs->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);
	connection.subscribeEvents();


	// start an httpConnection
	// store it in the connection
	// manipulate Connection Events::Subscription and infuse http event handlers
	// obtain state from the server context
}


void	Http::Module::CallbackForcedClose(Conn::BaseConnection& connection)
{
	Http::Connection* httpConn = reinterpret_cast<Http::Connection*>(connection.accessAppLayerConn());

	// cleanup, Connection told me we are closing shop
	delete httpConn;
}