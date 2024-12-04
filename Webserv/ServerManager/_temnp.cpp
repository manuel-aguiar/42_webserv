/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _temnp.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:04:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/04 15:09:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig.hpp"

// C++ headers

void	ServerManager::prepareWorkers()
{
	Nginx_MemoryPool*	workerMemPool;
	ServerWorker*		serverWorker;
	size_t				listenerCount;

	m_workers.reserve(m_config.getMaxWorkers());
	for (size_t i = 0; i < m_workers.size(); i++)
	{
		workerMemPool = Nginx_MemoryPool::create(4096, 1);
		serverWorker = (ServerWorker*)workerMemPool->allocate(sizeof (ServerWorker));
		new (serverWorker) ServerWorker(*this, i, workerMemPool, m_globals);
	}
}

int freeAllAddrInfo(std::vector<t_addrinfo*>& allAddrInfo)
{
	for (size_t i = 0; i < allAddrInfo.size(); ++i)
		::freeaddrinfo(allAddrInfo[i]);
}

/*

	Use getaddrinfo on all ip:port combinations to extract all possible addresses to listen to.
	Counts the number of addresses as these will translate to listeningsockets
	Uses a std::set as a hashset to avoid doubled entries when setting up listening sockets:
	That means we need to getaddrinfo on all (one you can localhost and 127.0.0.1, it must be the DNS to resolve
	pointing to the same place), and then adding them to a hashSet to get all the unique combinations

		-> setup the lsitening sockets from all the unique combinations
	
	freeaddrinfo on the vector that holds all struct addrinfo linkedlists

*/

/*
	struct addrinfo {
	int             ai_flags;         
	int             ai_family;        
	int             ai_socktype;    
	int             ai_protocol;     
	socklen_t       ai_addrlen;       
	char            *ai_canonname;   
	struct sockaddr *ai_addr;        
	struct addrinfo *ai_next;       
	};
*/

struct AddrinfoPtrComparator
{
	bool operator()(const t_addrinfo* a, const t_addrinfo* b)
	{
		int memcmp;

		if (a->ai_flags != b->ai_flags)
			return (a->ai_flags < b->ai_flags);
		if (a->ai_family != b->ai_family)
			return (a->ai_family < b->ai_family);
		if (a->ai_socktype != b->ai_socktype)
			return (a->ai_socktype < b->ai_socktype);	
		if (a->ai_protocol != b->ai_protocol)
			return (a->ai_protocol < b->ai_protocol);	
		if (a->ai_addrlen != b->ai_addrlen)
			return (a->ai_addrlen < b->ai_addrlen);
		if (a->ai_canonname != b->ai_canonname)
		{
			if (a->ai_canonname == NULL)
				return (true);
			if (b->ai_canonname == NULL)
				return (false);
			memcmp = std::strcmp(a->ai_canonname, b->ai_canonname);
			if (memcmp != 0)
				return (memcmp < 0);
		}
		if (a->ai_addr != b->ai_addr)
		{
			if (a->ai_addr == NULL)
				return (true);
			if (b->ai_addr == NULL)
				return (false);
			memcmp = std::memcmp(a->ai_addr, b->ai_addr, a->ai_addrlen);
			if (memcmp != 0)
				return (memcmp < 0);
		}
		return (false);
	}
};


	struct ListenerPtrComparator {
		bool operator()(const t_listeners* a, const t_listeners* b) const
		{	
			if (a->first != b->first)
            	return (a->first < b->first);
			return (a->second < b->second);
		}
	};

	std::set<const t_listeners*, ListenerPtrComparator>	filterListeners;


int	setupAllAddrinfo(const ServerConfig& config, 
					std::set<const t_addrinfo*, AddrinfoPtrComparator>& unique_AddrInfo,
					std::vector<const t_addrinfo*>& all_AddrInfo, 
					int socktype, int addrFamily, int backlog)
{
	t_addrinfo          						*res;
	t_addrinfo          						*cur;
	t_addrinfo          						hints;
	int											status;
	int											totalCount = 0;

	typedef std::map<std::string, ServerBlocks> t_blocks;
	const t_blocks& 							blocks = config.getServerBlocks();

	struct ListenerPtrComparator {
		bool operator()(const t_listeners* a, const t_listeners* b) const
		{	
			if (a->first != b->first)
            	return (a->first < b->first);
			return (a->second < b->second);
		}
	};

	std::set<const t_listeners*, ListenerPtrComparator>	filterListeners;

	hints = (t_addrinfo){};
	hints.ai_family = addrFamily;
	hints.ai_socktype = socktype;

	for (t_blocks::const_iterator iter = blocks.begin(); iter != blocks.end(); iter++)
	{
		const std::vector<t_listeners>&	listeners = iter->second.getListeners();
		for (size_t i = 0; i < listeners.size(); i++)
		{
			if (filterListeners.insert(&listeners[i]).second == false)
				continue ;

			status = getaddrinfo
			(
				listeners[i].first.c_str(), 
				listeners[i].second.c_str(),
				&hints,
				&res
			);

			if (status != 0)
			{
				// print something, maybe exit, return (-1) for error ??
				continue ;
			}
			all_AddrInfo.push_back(res);
			for(cur = res; cur != NULL; cur = cur->ai_next)
			{
				if (unique_AddrInfo.insert(cur).second == false)
					continue ;			

			}
				
		}
	}
	return (totalCount);
}

void	ServerManager::setupSingleListener(const t_ip_str& ip, const t_port_str& port, int socktype, int addrFamily, int backlog)
{
	ListeningSocket     *listener;
	t_addrinfo          hints;
	t_addrinfo          *res;
	t_addrinfo          *cur;
	t_sockaddr*         addr;

	hints = (t_addrinfo){};
	hints.ai_family = addrFamily;
	hints.ai_socktype = socktype;

	int status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);

	if (status != 0)
	{
		m_globals->logStatus("getaddrinfo(): " + std::string(gai_strerror(status)));
		return ;
	}

	for(cur = res; cur != NULL; cur = cur->ai_next)
	{
		listener = (ListeningSocket *)m_memPool->allocate(sizeof(ListeningSocket), true);
		new (listener) ListeningSocket(*this, m_globals);

		addr = (t_sockaddr *)m_memPool->allocate(cur->ai_addrlen, true);
		std::memcpy(addr, cur->ai_addr, cur->ai_addrlen);
		
		listener->setAddr(addr);
		listener->setSockType(cur->ai_socktype);
		listener->setProtocol(cur->ai_protocol);
		listener->setAddrlen(cur->ai_addrlen);
		listener->setBacklog(backlog);
		listener->setPort(::ntohs(((struct sockaddr_in *)cur->ai_addr)->sin_port));
		listener->accessEvent().setHandlerFunction_and_Data(&ListeningSocket::EventAccept, listener);
		listener->accessEvent().setFlags(EPOLLIN);


		if (listener->open())
			m_listeners.emplace_back(*listener);
		else
		{
			listener->~ListeningSocket();
			continue ;
		}
		//std::cout << "added listener: " << listener->m_sockfd << "and event fd " << listener->m_myEvent.m_fd << std::endl;
		m_eventManager.addEvent(listener->getSocket(), listener->getEvent());

	}
	freeaddrinfo(res);
	return (0);	
}

void	ServerManager::setupListeners()
{
	std::vector<t_addrinfo*> 	allAddrInfo;
	int							listenerCount;

	listenerCount = setupAllAddrinfo(m_config, allAddrInfo, SOCK_STREAM, AF_INET, 100);				///must be replaced with correct values
	
}

