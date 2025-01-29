

# include "InternalConn.hpp"

InternalConn::InternalConn(InternalManager& connManager) :
	Conn::Connection(connManager)
{
}

InternalConn::~InternalConn()
{
	m_monitor.release(accessEventManager());
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
	m_monitor.reset(accessEventManager(), false);
	m_appLayer.reset();
}

void
InternalConn::mf_callAppLayerForceClose()
{
	if (m_appLayer.accessCloseCallback())
		m_appLayer.accessCloseCallback()(*this);
}
