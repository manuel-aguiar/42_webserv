

# include "Connection.hpp"
# include "../../Events/Manager/Manager.hpp"


namespace Conn
{
	void	Conn::Connection::subscribeEvents(bool isCalledFromEventLoop)
	{
		m_eventManager.startMonitoring(*m_eventSubs, isCalledFromEventLoop);
	}

	void	Conn::Connection::unsubscribeEvents(bool isCalledFromEventLoop)
	{
		m_eventManager.stopMonitoring(*m_eventSubs, isCalledFromEventLoop);
	}

	void	Conn::Connection::modifyEvents(bool isCalledFromEventLoop)
	{
		m_eventManager.updateEvents(*m_eventSubs, isCalledFromEventLoop);
	}	

}