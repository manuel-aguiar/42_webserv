
// targets
# include "../Manager/Manager.hpp"
# include "../Connection/Connection.hpp"

#include "AppLayerProtos.hpp"
# include "../../../Toolkit/TestHelpers/TestHelpers.h"
# include "../../../Toolkit/ThreadPool/ThreadPool.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

# include <unistd.h>

ProtoFastClose::ProtoFastClose(Ws::AppLayer::Type appLayer) : serveCount(0), appLayer(appLayer) {}

ProtoFastClose::~ProtoFastClose() {}

void ProtoFastClose::InitConnection(Conn::Connection& conn)
{
    ProtoFastClose* fastClose = reinterpret_cast<ProtoFastClose*>(
        conn.accessServerContext().getAppLayerModule(conn.accessSocket().getBindInfo().appLayer)
    );

    unsigned char response = 200;
    ::write(conn.accessSocket().getSockFd(), &response, 1);
    conn.close();

    fastClose->serveCount++;
}

///////////////////////////////////////////////////////

ProtoNeverClose::ProtoNeverClose(Ws::AppLayer::Type appLayer) : serveCount(0), appLayer(appLayer) {}
ProtoNeverClose::~ProtoNeverClose() {}

void ProtoNeverClose::InitConnection(Conn::Connection& conn)
{
	conn.accessAppLayer().setCloseCallback(&ProtoNeverClose::ForcedClose);

    unsigned char response = 200;
    ::write(conn.accessSocket().getSockFd(), &response, 1);
}

void ProtoNeverClose::ForcedClose(Conn::Connection& conn)
{
	ProtoNeverClose* neverClose = reinterpret_cast<ProtoNeverClose*>(
        conn.accessServerContext().getAppLayerModule(conn.accessSocket().getBindInfo().appLayer)
    );

	neverClose->serveCount++;
	conn.close();
}

////////////////////////////////////////////////////////

SingleRequest::SingleRequest(ProtoSingleRequest& module, Conn::Connection& conn) :  conn(conn), module(module) {}

SingleRequest::~SingleRequest() {}

void SingleRequest::ReadWrite_Callback(Events::Subscription& conn)
{
    SingleRequest* request = reinterpret_cast<SingleRequest*>(conn.accessUser());
    request->ReadWrite();
}

void SingleRequest::ReadWrite()
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
        unsigned char request;
        ::read(conn.accessSocket().getSockFd(), &request, 1);
        conn.accessEvent().setMonitoredEvents(Events::Monitor::WRITE | Events::Monitor::ERROR | Events::Monitor::HANGUP);
        conn.updateMonitoring(false);
    }
    if (triggeredEvents & Events::Monitor::WRITE)
    {
        unsigned char response = 200;
        ::write(conn.accessSocket().getSockFd(), &response, 1);
        conn.close();
        delete (this);
    }
}

ProtoSingleRequest::ProtoSingleRequest(const Ws::AppLayer::Type type) 
    : serveCount(0), appLayer(type)
{
    // Empty constructor
}

ProtoSingleRequest::~ProtoSingleRequest() 
{
    // Empty destructor
}

void ProtoSingleRequest::InitConnection(Conn::Connection& conn) 
{
    ProtoSingleRequest* module = reinterpret_cast<ProtoSingleRequest*>(
        conn.accessServerContext().getAppLayerModule(conn.accessSocket().getBindInfo().appLayer)
    );

    SingleRequest* request = new SingleRequest(*module, conn);
    conn.accessAppLayer().setConn(request);
    conn.accessAppLayer().setCloseCallback(&ProtoSingleRequest::ForcedClose);
    conn.accessEvent().setUser(request);
    conn.accessEvent().setCallback(&SingleRequest::ReadWrite_Callback);
    conn.accessEvent().setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP);
    conn.startMonitoring(false);
}

void ProtoSingleRequest::ForcedClose(Conn::Connection& conn) 
{
    SingleRequest* request = reinterpret_cast<SingleRequest*>(conn.accessAppLayer().accessConn());

    delete (request);
    conn.close();
}