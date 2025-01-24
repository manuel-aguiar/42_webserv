

# include "ServerWorker.hpp"

// getters

int							ServerWorker::getID() const
{
	return (m_myID);
}

const ServerManager&		ServerWorker::getServerManager() const
{
	return (m_serverManager);
}

//const CgiManager&			ServerWorker::getCgiManager() const
//{
//	return (m_cgiManager);
//}

const Manager&	ServerWorker::getConnManager() const
{
	return (m_connManager);
}

const EventManager&			ServerWorker::getEventManager() const
{
	return (m_eventManager);
}

const DynArray<ListeningSocket*, Nginx_PoolAllocator<ListeningSocket*> >&
							ServerWorker::getListeners()		const
{
	return (m_listeners);
}								

const Nginx_MemoryPool&		ServerWorker::getMemPool() const
{
	return (m_memPool);
}


//accessors
ServerManager&				ServerWorker::accessServerManager()
{
	return (m_serverManager);
}

//CgiManager&					ServerWorker::accessCgiManager()
//{
//	return (m_cgiManager);
//}

Manager&			ServerWorker::accessConnManager()
{
	return (m_connManager);
}

EventManager&				ServerWorker::accessEventManager()
{
	return (m_eventManager);
}

DynArray<ListeningSocket *, Nginx_PoolAllocator<ListeningSocket *> >&		ServerWorker::accessListeners()
{
	return (m_listeners);
}

Nginx_MemoryPool&			ServerWorker::accessMemPool()
{
	return (m_memPool);
}
