
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