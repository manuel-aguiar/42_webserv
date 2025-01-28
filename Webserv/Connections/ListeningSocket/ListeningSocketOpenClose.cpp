

# include "ListeningSocket.hpp"
# include "../ImplManager/ImplManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"


int		ListeningSocket::open()
{
	int options;

	m_sockfd = ::socket(m_info.family, m_info.socktype, m_info.proto);
	if (m_sockfd == -1)
	{
		mf_accessGlobals().logError("ListeningSocket::open, socket(): " + std::string(strerror(errno)));
		return (0);
	}

	if (!FileDescriptor::setCloseOnExec_NonBlocking(m_sockfd))
	{
		mf_accessGlobals().logError("ListeningSocket::open(), setCloseOnExec_NonBlocking(): " + std::string(strerror(errno)));
		return (0);
	}

	#ifdef SO_REUSEPORT
		options = SO_REUSEADDR | SO_REUSEPORT;
	#else
		options = SO_REUSEADDR;
	#endif

	if (::setsockopt(m_sockfd, SOL_SOCKET, options, &options, sizeof(options)) == -1)
	{
		mf_accessGlobals().logError("ListeningSocket::open(), setsockopt(): " + std::string(strerror(errno)));
		return (0);
	}	
	if (::bind(m_sockfd, (struct sockaddr*)(&m_info.addr), m_info.addrlen) == -1)
	{
		mf_accessGlobals().logError("ListeningSocket::bind(), bind(): " + std::string(strerror(errno)));
		return (0);
	}
	if (::listen(m_sockfd, m_backlog) == -1)
	{
		mf_accessGlobals().logError("ListeningSocket::listen(): listen():" + std::string(strerror(errno)));
		return (0);
	}

	ASSERT_EQUAL(m_eventSubs == NULL, true, "ListeningSocket::open(), listening socket already opened, that can't be");
	m_eventSubs = mf_accessEventManager().acquireSubscription();
	ASSERT_EQUAL(m_eventSubs != NULL, true, "ListeningSocket::open(), acquireSubscription() failed, should have enough for all");
	m_eventSubs->setFd(m_sockfd);
	m_eventSubs->setMonitoredEvents(Events::Monitor::READ | Events::Monitor::ERROR | Events::Monitor::HANGUP | Events::Monitor::EDGE_TRIGGERED);
	m_eventSubs->setUser(this);
	m_eventSubs->setCallback(EventCallbackAccept);
	mf_accessEventManager().startMonitoring(*m_eventSubs, false);

	return (1);
}

void	ListeningSocket::close()
{
	if (m_eventSubs)
	{
		mf_accessEventManager().stopMonitoring(*m_eventSubs, false);
		mf_accessEventManager().returnSubscription(*m_eventSubs);
		m_eventSubs = NULL;
	}
	if (m_sockfd != -1)
	{
		::close(m_sockfd);
		m_sockfd = -1;
	}
}
