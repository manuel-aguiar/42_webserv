

# include "HttpModule.hpp"
# include "../HttpConnection/HttpConnection.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
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

        httpConnection->setMyTCP(connection);

        connection.appLayer_setCloseCallback(Http::Module::ForcedClose);
        connection.appLayer_setConn(httpConnection);

        connection.events_setUser(httpConnection);
        connection.events_setCallback(Http::Connection::ReadWrite_Callback);
        connection.events_setMonitoredEvents(Events::Monitor::READ | 
                                            Events::Monitor::WRITE | 
                                            Events::Monitor::ERROR | 
                                            Events::Monitor::HANGUP);
        
        ServerConfig& serverConfig = *connection.accessServerContext().getServerConfig();                                    
        int timeout = serverConfig.getTimeoutFullHeader();
        httpConnection->setMyTimer(module.insertTimer(Timer::now() + timeout, *httpConnection));

        connection.events_startMonitoring(true);
    }

    void
    Module::ForcedClose(Conn::Connection& connection)
    {
        Http::Connection*   httpConnection = reinterpret_cast<Http::Connection*>(connection.appLayer_accessConn());

        httpConnection->closeConnection();
    }

}	// end of http namespace
