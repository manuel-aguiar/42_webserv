
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
        conn.accessServerContext().getAppLayerModule(conn.accessSocket().getBindInfo().appLayer)
    );

    unsigned char response = 200;
    ::write(conn.accessSocket().getSockFd(), &response, 1);
    conn.close();

    fastClose->serveCount++;
}

///////////////////////////////////////////////////////

Server_NeverCloseModule::Server_NeverCloseModule(Ws::AppLayer::Type appLayer) : serveCount(0), appLayer(appLayer) {}
Server_NeverCloseModule::~Server_NeverCloseModule() {}

void Server_NeverCloseModule::InitConnection(Conn::Connection& conn)
{
	conn.accessAppLayer().setCloseCallback(&Server_NeverCloseModule::ForcedClose);

    unsigned char response = 200;
    ::write(conn.accessSocket().getSockFd(), &response, 1);
}

void Server_NeverCloseModule::ForcedClose(Conn::Connection& conn)
{
	Server_NeverCloseModule* neverClose = reinterpret_cast<Server_NeverCloseModule*>(
        conn.accessServerContext().getAppLayerModule(conn.accessSocket().getBindInfo().appLayer)
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

void Server_MathRequest::ReadWrite()
{
    int triggeredEvents = conn.accessEvent().getTriggeredEvents();

    if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
    {
        conn.close();
        delete (this);
        return ;
    }
    if (triggeredEvents & Events::Monitor::READ)
    {
        ::read(conn.accessSocket().getSockFd(), &received, 1);
        conn.accessEvent().setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);
        conn.updateMonitoring(false);
    }
    if (triggeredEvents & Events::Monitor::WRITE)
    {
        received = (received + 3) % (256);
        ::write(conn.accessSocket().getSockFd(), &received, 1);
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
        conn.accessServerContext().getAppLayerModule(conn.accessSocket().getBindInfo().appLayer)
    );

    Server_MathRequest* request = new Server_MathRequest(*module, conn);
    conn.accessAppLayer().setConn(request);
    conn.accessAppLayer().setCloseCallback(&Server_MathModule::ForcedClose);
    conn.accessEvent().setUser(request);
    conn.accessEvent().setCallback(&Server_MathRequest::ReadWrite_Callback);
    conn.accessEvent().setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
    conn.startMonitoring(false);
}

void Server_MathModule::ForcedClose(Conn::Connection& conn) 
{
    Server_MathRequest* request = reinterpret_cast<Server_MathRequest*>(conn.accessAppLayer().accessConn());

    delete (request);
    conn.close();
}