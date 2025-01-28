

# include "InternalConn.hpp"
# include "../../Events/Subscription/Subscription.hpp"

InternalConn::InternalConn(ImplManager& connManager) :
	Conn::Connection(connManager)
{
}

InternalConn::~InternalConn()
{
}


void
InternalConn::ForcedClose()
{
	if (m_socket.getSockFd() == -1)
		return ;
	mf_callAppLayerForceClose();
	Conn::Connection::close();
}

void
InternalConn::reset()
{
	m_socket.reset();
	m_monitor.reset(false);
	m_appLayer.reset();
}

void
InternalConn::mf_callAppLayerForceClose()
{


}
