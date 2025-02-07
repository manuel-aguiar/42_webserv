

# include "InternalConn.hpp"
# include "../../Events/Subscription/Subscription.hpp"

InternalConn::InternalConn(Events::Manager& eventManager, ServerContext& context, InternalManager& connManager) :
	Conn::Connection(eventManager, context, &connManager)
{
}

InternalConn::~InternalConn()
{
	m_monitor.release();
}

void
InternalConn::forcedClose()
{
	if (m_socket.getSockFd() == Ws::FD_NONE)
		return ;
	mf_callAppLayerForceClose();
	Conn::Connection::close();
}

#include <iostream>
void
InternalConn::reset()
{
	//std::cout << "socket fd: " << m_socket.getSockFd() << ", event fd: " << m_monitor.accessEvent().getFd() << std::endl;
	m_socket.reset();
	m_monitor.reset(false);
	m_appLayer.reset();
}

void
InternalConn::mf_callAppLayerForceClose()
{
	if (m_appLayer.accessCloseCallback())
		m_appLayer.accessCloseCallback()(*this);
}

void InternalConn::prepareDispatch()
{
	m_monitor.accessEvent().setFd(m_socket.getSockFd());
}

ConnInfo&
InternalConn::accessSocket()
{
	return (m_socket);
}
