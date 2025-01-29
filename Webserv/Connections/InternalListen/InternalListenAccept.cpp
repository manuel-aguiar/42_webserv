
# include "InternalListen.hpp"
# include "../InternalManager/InternalManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Globals/Globals.hpp"



void  InternalListen::accept()
{
	InternalConn*	connection;

	connection = m_connManager._Listener_ProvideConnection();

	if (!connection)
	{
		m_monitor.unsubscribe(mf_accessEventManager(), false);
		return (m_connManager._Listener_MoveToPendingAccept(*this));
	}

	int res = m_listener.accept(m_socket, connection->accessSocket());

	ASSERT_EQUAL(res != -1, true, "ListeningSocket::accept(), beAccepted() failed, this handler should only be called when we are sure there is a connection via epoll");	

	mf_acceptInternal(*connection);
}

int InternalListen::acceptPending(InternalConn& connection)
{
	int result = m_listener.accept(m_socket, connection.accessSocket());

	if (result == -1)
	{
		//listener has nobody waiting
		m_monitor.subscribe(mf_accessEventManager(), false);
		m_connManager._ReturnConnection(connection);
		return (result);
	}
	return (mf_acceptInternal(connection));
}

int
InternalListen::mf_acceptInternal(InternalConn& connection)
{
	mf_accessServerContext().getAppLayerInit(m_socket.getBindInfo().appLayer)(connection);
	return (1);
}