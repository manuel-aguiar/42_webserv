

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

InternalConn::InternalConn(const InternalConn& copy) :
	Conn::Connection(copy) {}

InternalConn&
InternalConn::operator=(const InternalConn& assign)
{
	if (this == &assign)
		return (*this);
	Conn::Connection::operator=(assign);
	return (*this);
}

void
InternalConn::forcedClose()
{
	if (m_peerInfo.sockfd == Ws::FD_NONE)
		return ;
	mf_callAppLayerForceClose();
	Conn::Connection::close();
}

void
InternalConn::reset()
{
	//std::cout << "socket fd: " << m_socket.getSockFd() << ", event fd: " << m_monitor.accessEvent().getFd() << std::endl;
	m_peerInfo.reset();
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
	m_monitor.accessEvent().setFd(m_peerInfo.sockfd);
}

ConnInfo&
InternalConn::accessConnInfo()
{
	return (m_peerInfo);
}
