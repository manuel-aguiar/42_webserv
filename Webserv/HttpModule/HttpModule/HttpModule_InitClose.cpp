

# include "HttpModule.hpp"
# include "../HttpConnection/HttpConnection.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Globals/Globals.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

# include <arpa/inet.h>

void    logConnection(Globals& globals, Conn::Connection& connection)
{
    const Ws::BindInfo& peer = connection.info_getPeerInfo();
    const Ws::BindInfo& listen = connection.info_getListenInfo();
    char ipPeer[INET_ADDRSTRLEN];
    char ipListen[INET_ADDRSTRLEN];

    ::inet_ntop(AF_INET, &peer.addr.sockaddr_in.sin_addr, ipPeer, INET_ADDRSTRLEN);
    int portPeer = ::ntohs(peer.addr.sockaddr_in.sin_port);

    ::inet_ntop(AF_INET, &listen.addr.sockaddr_in.sin_addr, ipListen, INET_ADDRSTRLEN);
    int portListen = ::ntohs(listen.addr.sockaddr_in.sin_port);

    std::string message = "connection from " + std::string(ipPeer) + ":" 
                            + StringUtils::to_string(portPeer) 
                            + " on listener " 
                            + std::string(ipListen) + ":" 
                            + StringUtils::to_string(portListen);
    globals.logStatus(message);
    std::cout << message << std::endl;
}

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

        httpConnection->prepareConnection(connection);
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
        httpConnection->setMyTimer(module.insertTimer(Timer::now() + timeout, *httpConnection), Http::Connection::Timeout::FULL_HEADER);

        connection.events_startMonitoring(true);
        
        Globals& globals = *connection.accessServerContext().getGlobals();
        logConnection(globals, connection);
    }

    void
    Module::ForcedClose(Conn::Connection& connection)
    {
        Http::Connection*   httpConnection = reinterpret_cast<Http::Connection*>(connection.appLayer_accessConn());

        httpConnection->closeConnection();
    }

}	// end of http namespace
