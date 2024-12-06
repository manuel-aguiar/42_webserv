/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagerPrepareWorkers.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:04:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 10:50:17 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig.hpp"

// C++ headers




/*

	Helper struct for comparint t_listeners when hashhing for unique values
	Not needed anywhere else in the project

*/
struct ListenerPtrComparator {
	bool operator()(const t_listeners* a, const t_listeners* b) const
	{
		if (a->first != b->first)
			return (a->first < b->first);
		return (a->second < b->second);
	}
};

/*

	A Comparator struct that we use on std::set to make sure we are not trying to build sockets of addrinfo that are
	exactly the same.

	Not needed anywhere else in the project

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

/*

	Use getaddrinfo on all ip:port combinations to extract all possible addresses to listen to.
	Counts the number of addresses as these will translate to listeningsockets
	Uses a std::set as a hashset to avoid doubled entries when setting up listening sockets:
	That means we need to getaddrinfo on all (one you can localhost and 127.0.0.1, it must be the DNS to resolve
	pointing to the same place), and then adding them to a hashSet to get all the unique combinations

		-> we also use another hashSet just to make sure we don't call getaddrinfo with 2 equal t_listeners.
		Not as relevant but getaddrinfo is an expensive function that uses like 50 syscalls, not kiding.
		I think (i hope) a std::set insert is lighter than that. This is deffinitely not relevant for the runtime

		-> setup the lsitening sockets from all the unique combinations

	freeaddrinfo on the vector that holds all struct addrinfo linkedlists

*/

static int getAddrInfo_Free(std::vector<t_addrinfo*>& allAddrInfo)
{
	for (size_t i = 0; i < allAddrInfo.size(); ++i)
		::freeaddrinfo(allAddrInfo[i]);
}

static int	getAddrInfo_Setup(const ServerConfig& 									config,
					std::set<const t_addrinfo*, AddrinfoPtrComparator>& 	unique_AddrInfo,
					std::vector<t_addrinfo*>& 								allLists_AddrInfo,
					int socktype, int addrFamily, int backlog)
{
	t_addrinfo          						*res;
	t_addrinfo          						*cur;
	t_addrinfo          						hints;
	int											status;

	typedef std::map<std::string, ServerBlocks> t_blocks;
	const t_blocks& 							blocks = config.getServerBlocks();



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

			status = ::getaddrinfo
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
			allLists_AddrInfo.push_back(res);
			for(cur = res; cur != NULL; cur = cur->ai_next)
				unique_AddrInfo.insert(cur);
		}
	}
	return (unique_AddrInfo.size());
}

/*
	Function to prepare the ServerWorker instances for battle.
	Takes config, initializes server workers, prepares listening sockets, connections, eventmanagers
	etc

	The process between isntantiating workers and setting up listening sockets could be separated.
	However, for cache locality, i wnat to make sure that the memory pool expands itself close together
	for the worker and that its own listeningsockets/addrinfo's are close together. so, to avoid entangling
	the memory between workers, i choose to do it all in one go.

	Comments were left in an attempt to explain what each part does.

	Gets all getaddrinfo structs from all ip:port combos obtained from config, and count all the unique ones.
	ServerWorkers reserve the space for that ammount of listening sockets.
	Actual listening sockets are allocated inside the worker's memory pool such that
	each one call allocate their memory addresses write next to themselves (should be better for cache)
	Each ListeningSocket also copies their addrinfo onto their instance.

	After all listening sockets are setup, we can free all addrinfo structs allocated by ::getaddrinfo()

	WARNING: NO NULL CHECKS YET FOR MEMORY FAILURE WHEN MEMORYPOOL EXPANDS

*/
void	ServerManager::prepareWorkers()
{
	Nginx_MemoryPool*									workerMemPool;

	//for getaddrinfo
	std::set<const t_addrinfo*, AddrinfoPtrComparator> 	unique_Addrinfo;
	std::vector<t_addrinfo*> 							allLists_Addrinfo;
	ListeningSocket*									newListener;
	size_t												listenerCount;

	m_workers.reserve(m_config.getMaxWorkers());

	listenerCount = getAddrInfo_Setup(m_config, unique_Addrinfo, allLists_Addrinfo, SOCK_STREAM, AF_INET, 100);				///must be replaced with correct values

	for (size_t i = 0; i < m_workers.size(); i++)
	{
		// create a memorypool for the worker, allocate itself in it, save pointer in ServerManager
		workerMemPool = Nginx_MemoryPool::create(4096, 1);
		m_workers[i] = (ServerWorker*)workerMemPool->allocate(sizeof (ServerWorker));
		new (m_workers[i]) ServerWorker(*this, i, workerMemPool, m_globals);

		// worker sets up its own listeners, inside its own memorypool
		m_workers[i]->accessListeners().reserve(listenerCount);
		for (std::set<const t_addrinfo*, AddrinfoPtrComparator>::iterator iter = unique_Addrinfo.begin(); iter != unique_Addrinfo.end(); ++iter)
		{
			newListener = (ListeningSocket*)m_workers[i]->accessMemPool().allocate(sizeof(ListeningSocket));
			new (newListener) ListeningSocket(*m_workers[i], **iter, m_globals);

			/*
				Here one should check m_config to see what protocol module and connection initializer
				should be passed on to the listening socket. For now just straight assign the HttpModules
			*/
			newListener->setProtoModule(m_protoModules[HTTP_MODULE]);
			newListener->setInitProtocolConnection(m_initProtoConnection[HTTP_MODULE]);

			m_workers[i]->accessListeners().emplace_back(newListener);
		}
	}

	getAddrInfo_Free(allLists_Addrinfo);
}
