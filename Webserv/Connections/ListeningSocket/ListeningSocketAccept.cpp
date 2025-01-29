
# include "ListeningSocket.hpp"
# include "../InternalManager/InternalManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Globals/Globals.hpp"



void  ListeningSocket::accept()
{
	InternalConn*	connection;

	connection = m_connManager._Accepter_ProvideConnection();

	if (!connection)
	{
		m_monitor.unsubscribe(mf_accessEventManager(), false);
		return (m_connManager._Accepter_MoveToPendingAccept(*this));
	}

	int res = m_accepter.accept(m_socket, connection->accessSocket());

	ASSERT_EQUAL(res != -1, true, "ListeningSocket::accept(), beAccepted() failed, this handler should only be called when we are sure there is a connection via epoll");	

	mf_acceptInternal(*connection);
}

int ListeningSocket::acceptPending(InternalConn& connection)
{
	int result = m_accepter.accept(m_socket, connection.accessSocket());

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
ListeningSocket::mf_acceptInternal(InternalConn& connection)
{
	mf_accessServerContext().getAppLayerInit(m_socket.getBindInfo().appLayer)(connection);
	return (1);
}