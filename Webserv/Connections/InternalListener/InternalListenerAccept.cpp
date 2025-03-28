
# include "InternalListener.hpp"
# include "../InternalManager/InternalManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Globals/Globals.hpp"


void  InternalListener::accept()
{
	InternalConn*	connection;

	while (1)
	{
		connection = m_connManager._Accepter_ProvideConnection();
		if (!connection)
		{
			m_monitor.unsubscribe(false);
			return (m_connManager._Accepter_MoveToPendingAccept(*this));
		}
		if (m_listener.accept(connection->accessConnInfo()) == Ws::FD_NONE)
		{
			m_connManager._ReturnConnection(*connection);
			return ;
		}
		mf_acceptInternal(*connection);
	}
}

int InternalListener::acceptPending(InternalConn& connection)
{
	int result = m_listener.accept(connection.accessConnInfo());

	if (result == -1)
	{
		m_monitor.subscribe(false);
		return (result);
	}
	return (mf_acceptInternal(connection));
}

int
InternalListener::mf_acceptInternal(InternalConn& connection)
{
	Ws::AppLayer::Type initializer = m_listener.getConnInfo().peerInfo.appLayer;

	connection.prepareDispatch();
	mf_accessServerContext().getAppLayerInit(initializer)(connection);
	return (1);
}
