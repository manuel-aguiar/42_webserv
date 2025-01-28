
# include "ListeningSocket.hpp"
# include "../ImplManager/ImplManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

void ListeningSocket::EventCallbackAccept(Events::Subscription& callback)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(callback.accessUser());
	listener->accept();
}

void  ListeningSocket::accept()
{
	InternalConn*	connection;

	connection = m_connManager._Listener_ProvideConnection();
	if (!connection)
	{
		mf_accessEventManager().stopMonitoring(*m_eventSubs, false);
		return (m_connManager._Listener_MoveToPendingAccept(*this));
	}

	int res = connection->beAccepted(m_sockfd, m_info.socktype, m_info.proto, m_info.appLayer);
	ASSERT_EQUAL(res != -1, true, "ListeningSocket::accept(), beAccepted() failed, this handler should only be called when we are sure there is a connection via epoll");	

	mf_acceptInternal(*connection);
}

int ListeningSocket::acceptPending(InternalConn& connection)
{
	int result = connection.beAccepted(m_sockfd, m_info.socktype, m_info.proto, m_info.appLayer);
	if (result == -1)
	{
		//listener has nobody waiting
		mf_accessEventManager().startMonitoring(*m_eventSubs, false);
		m_connManager._ReturnConnection(connection);
		return (result);
	}
	return (mf_acceptInternal(connection));
}

int
ListeningSocket::mf_acceptInternal(InternalConn& connection)
{
	if (!FileDescriptor::setCloseOnExec_NonBlocking(connection.getSocket()))
	{
		mf_accessGlobals().logError("ListeningSocket::mf_acceptInternal(), setCloseOnExec_NonBlocking(): " + std::string(strerror(errno)));
		return (-1);
	}
	mf_accessServerContext().getAppLayerInit(m_info.appLayer)(connection);
	return (1);
}