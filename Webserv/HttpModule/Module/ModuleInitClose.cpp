

# include "Module.hpp"
# include "../Connection/Connection.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

namespace Http
{
    void
    Module::InitConnection(Conn::Connection& connection)
    {

        Http::Module& module = *reinterpret_cast<Http::Module*>(
        connection.accessServerContext().getAppLayerModule(Ws::AppLayer::HTTP));

        Http::Connection* httpConnection = module.acquireConnection();
        
        ASSERT_EQUAL(httpConnection != NULL, true, 
        "Http::Module::InitConnection - failed to acquire connection, there should be connections available for everyone");

        // giving httpConn access to Conn::Connection (fd, ip, port, sockaddr)
        httpConnection->setMyTCP(&connection);

        // informing server connection who we are
        connection.appLayer_setCloseCallback(Http::Module::ForcedClose);
        connection.appLayer_setConn(httpConnection);

        // preping events
        connection.events_setUser(httpConnection);
        connection.events_setCallback(Http::Connection::ReadWrite_Callback);
        connection.events_setMonitoredEvents(Events::Monitor::READ | 
                                            Events::Monitor::WRITE | 
                                            Events::Monitor::ERROR | 
                                            Events::Monitor::HANGUP);
                                            
        // no need to setup fd, listening socket preps this before hand, it's ready to go
        // called with true, this is all from the event loop
        connection.events_startMonitoring(true);
    }

    void
    Module::ForcedClose(Conn::Connection& connection)
    {
        Http::Connection*   httpConnection = reinterpret_cast<Http::Connection*>(connection.appLayer_accessConn());

        httpConnection->close();
    }

}	// end of http namespace