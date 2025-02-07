
// targets
# include "../Manager/Manager.hpp"
# include "../Connection/Connection.hpp"

# include "_ServerProtos.hpp"

# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

# include <unistd.h>

Server_FastCloseModule::Server_FastCloseModule(Ws::AppLayer::Type appLayer) : serveCount(0), appLayer(appLayer) {}

Server_FastCloseModule::~Server_FastCloseModule() {}

void Server_FastCloseModule::InitConnection(Conn::Connection& conn)
{
    Server_FastCloseModule* fastClose = reinterpret_cast<Server_FastCloseModule*>(
        conn.accessServerContext().getAppLayerModule(Ws::AppLayer::HTTP)
    );

    unsigned char response = 200;
    ::write(conn.info_getFd(), &response, 1);
    conn.close();

    fastClose->serveCount++;
}

///////////////////////////////////////////////////////

Server_NeverCloseModule::Server_NeverCloseModule(Ws::AppLayer::Type appLayer) : serveCount(0), appLayer(appLayer) {}
Server_NeverCloseModule::~Server_NeverCloseModule() {}

void Server_NeverCloseModule::InitConnection(Conn::Connection& conn)
{
	conn.appLayer_setCloseCallback(&Server_NeverCloseModule::ForcedClose);

    unsigned char response = 200;
    ::write(conn.info_getFd(), &response, 1);
}

void Server_NeverCloseModule::ForcedClose(Conn::Connection& conn)
{
	Server_NeverCloseModule* neverClose = reinterpret_cast<Server_NeverCloseModule*>(
        conn.accessServerContext().getAppLayerModule(Ws::AppLayer::HTTP)
    );

	neverClose->serveCount++;
	conn.close();
}

////////////////////////////////////////////////////////

Server_MathRequest::Server_MathRequest(Server_MathModule& module, Conn::Connection& conn) : received(0), conn(conn), module(module) {}

Server_MathRequest::~Server_MathRequest() {}

void Server_MathRequest::ReadWrite_Callback(Events::Subscription& conn)
{
    Server_MathRequest* request = reinterpret_cast<Server_MathRequest*>(conn.accessUser());
    request->ReadWrite();
}


/*
    Client hangup or error, close connection. we are a server, if the client has closed then
    our purpose is fulfilled.

    The protocol InitConnection function subscribed read first, only (we have nothing to write,
    without knowing what the client wants). Ideally, do both at the same:
        if you are lucky you can read, parse and write all in one turn, which is faster. For
        this tests and to add unpredictability to patterns, we deliberately wait for another turn
        to write back. (could do nginx style: write, if fails, subscribe, FORBIDDEN BY SUBJECT :'()
        -> example below of nginx style with optimistic writing.
    
    After reading, update event to monitor writting (won't trigger this turn).
    next turn, write the modified value "pre-agreed" with the client ((client number + 3) % 256).
    Close connection and mark success for the transaction.

    self deleting -> dangerous, but we are sure the connection is closed, and we are done.
*/
void Server_MathRequest::ReadWrite()
{
    int triggeredEvents = conn.events_getTriggeredEvents();

    if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
    {
        conn.close();
        delete (this);
        return ;
    }
    if (triggeredEvents & Events::Monitor::READ)
    {
              
        ::read(conn.info_getFd(), &received, 1);
        conn.events_setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);
        conn.events_updateMonitoring(false);
        
    
    /*
        //////////////////////////////////////////////////////////  
        // This is the optimistic writing version, NGINX style: //
        //////////////////////////////////////////////////////////

        ::read(conn.info_getFd(), &received, 1);  
        received = (received + 3) % (256);
        int bytesWritten = ::write(conn.info_getFd(), &received, 1);
        if (bytesWritten == -1)
        {
            // writing failed (client socket is full), start monitoring writing for the next turn
            conn.events_setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);
            conn.events_updateMonitoring(false);
            return ;
        }
        else
        {
            //success, able to write back in the same turn, close and mark success
            conn.close();
            this->module.serveCount++;
            delete (this);
        }
    */ 

    }
    if (triggeredEvents & Events::Monitor::WRITE)
    {
        received = (received + 3) % (256);
        ::write(conn.info_getFd(), &received, 1);
        conn.close();
        this->module.serveCount++;
        delete (this);
    }
}

Server_MathModule::Server_MathModule(const Ws::AppLayer::Type type) 
    : serveCount(0), appLayer(type)
{
    // Empty constructor
}

Server_MathModule::~Server_MathModule() 
{
    // Empty destructor
}

void Server_MathModule::InitConnection(Conn::Connection& conn) 
{
    Server_MathModule* module = reinterpret_cast<Server_MathModule*>(
        conn.accessServerContext().getAppLayerModule(Ws::AppLayer::HTTP)
    );

    Server_MathRequest* request = new Server_MathRequest(*module, conn);
    conn.appLayer_setConn(request);
    conn.appLayer_setCloseCallback(&Server_MathModule::ForcedClose);
    conn.events_setUser(request);
    conn.events_setCallback(&Server_MathRequest::ReadWrite_Callback);
    conn.events_setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
    conn.events_startMonitoring(false);
}

void Server_MathModule::ForcedClose(Conn::Connection& conn) 
{
    Server_MathRequest* request = reinterpret_cast<Server_MathRequest*>(conn.appLayer_accessConn());

    delete (request);
    conn.close();
}