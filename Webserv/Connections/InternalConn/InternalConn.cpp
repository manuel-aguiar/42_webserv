

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
	if (m_info.sockfd == Ws::FD_NONE)
		return ;
	mf_callAppLayerForceClose();
	Conn::Connection::close();
}

#include <iostream>
void
InternalConn::reset()
{
	//std::cout << "socket fd: " << m_socket.getSockFd() << ", event fd: " << m_monitor.accessEvent().getFd() << std::endl;
	m_info.reset();
	m_monitor.reset(false);
	m_appConn = NULL;
	m_appForceClose = NULL;
}

void
InternalConn::mf_callAppLayerForceClose()
{
	if (m_appForceClose)
		m_appForceClose(*this);
}

void InternalConn::prepareDispatch()
{
	m_monitor.accessEvent().setFd(m_info.sockfd);
}

ConnInfo&
InternalConn::accessConnInfo()
{
	return (m_info);
}
