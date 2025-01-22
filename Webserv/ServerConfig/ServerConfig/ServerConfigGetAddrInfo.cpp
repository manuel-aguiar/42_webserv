
# include "ServerConfig.hpp"
# include <cstddef>
# include <cstring>

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


struct DNSLookupHelper
{
	~DNSLookupHelper()
	{
		for (size_t i = 0; i < allAddrInfo.size(); ++i)
			::freeaddrinfo(allAddrInfo[i]);
	}

	std::set<t_listeners>								uniqueListeners;
	std::vector<t_addrinfo*>							allAddrInfo;

	std::multimap<const t_listeners*, const u_sockaddr*, ListenerPtrComparator>	
														listenerToAddress;

	std::map<const t_addrinfo*, const u_sockaddr*, AddrinfoPtrComparator>	
														uniqueAddrInfo;

	typedef std::map<const t_addrinfo*, const u_sockaddr*, AddrinfoPtrComparator>::iterator t_addrIter;
	typedef std::multimap<const t_listeners*, const u_sockaddr*, ListenerPtrComparator>::iterator t_listenerIter;

	std::vector<u_sockaddr>								uniqueSockAddr;
};


static	void getAllListeners(const ServerConfig& config, DNSLookupHelper& helper)
{
	const std::vector<ServerBlock>&	serverBlocks = config.getServerBlocks();

	for (size_t i = 0; i < serverBlocks.size(); ++i)
	{
		const std::set<t_listeners>&	listeners = serverBlocks[i].getListeners();
		for (std::set<t_listeners>::const_iterator iter = listeners.begin(); iter != listeners.end(); ++iter)
			helper.uniqueListeners.insert(*iter);
	}
}

/*
	There are multiple combinations here:
		to listeners referring to the same t_addrinfo
		so, then to addrinfo referring to the same addrinfo
	We need: 
		a unique combination of u_sockaddr
		maintain separation so we can identify them back to the original listener
	
	so we multimap all listeners to their unique sockaddr, pre-computed from the unique addrinfo

	The final step is to iterate the listeners, and for all the unique addrinfo's that are linked to them,
	we push the unique sockaddr* to the serverblock, already typecasted to struct sockaddr* for
	consumption by the BlockFinder


*/

static bool get_AddrInfo_and_Map(DNSLookupHelper& helper)
{
	t_addrinfo          						*res;
	t_addrinfo          						*cur;
	t_addrinfo          						hints;

	for (std::set<t_listeners>::iterator iter = helper.uniqueListeners.begin(); iter != helper.uniqueListeners.end(); ++iter)
	{
		hints = (t_addrinfo){};
		hints.ai_family = AF_INET;    			//listener would tell me if it is ipv4 or ipv6
		hints.ai_socktype = SOCK_STREAM;		//listener would tell me if it is tcp or udp or something else

		if (::getaddrinfo(iter->first.c_str(), iter->second.c_str(), &hints, &res) != 0)
			return (false);
		helper.allAddrInfo.push_back(res);
		for (cur = res; cur != NULL; cur = cur->ai_next)
		{
			DNSLookupHelper::t_addrIter addrIter = helper.uniqueAddrInfo.find(cur);

			//unique
			if (addrIter == helper.uniqueAddrInfo.end())
			{
				u_sockaddr address = (u_sockaddr){};

				//adding unique address
				std::memcpy(&address, cur->ai_addr, cur->ai_addrlen);
				helper.uniqueSockAddr.push_back(address);

				//adding unique address to map
				std::pair<DNSLookupHelper::t_addrIter, bool> returnVal;
				returnVal = helper.uniqueAddrInfo.insert(std::pair<const t_addrinfo*, const u_sockaddr*>(cur, &(helper.uniqueSockAddr.back())));
				if (returnVal.second == false) // can't be false, veryfication before, but hey
					return (false);
				
				// connecting listener to u_sockaddr, where we inserted it
				helper.listenerToAddress.insert(std::pair<const t_listeners*, const u_sockaddr*>(&(*iter), returnVal.first->second));
			}
			else
				//we know were we inserted it, link it here
				helper.listenerToAddress.insert(std::pair<const t_listeners*, const u_sockaddr*>(&(*iter), addrIter->second));
		}
	}
	return (true);
}

/*
	Ok, making it simple, one step at a time.

	1) get all unique listeners from all serverblocks
	2) get all getaddrinfo linkedlists from all the unique listeners
		b) prepare the sockaddr structs from the unique addresses
		a) set a listener to unique sockaddr multimap
	3) assign the multiple unique sockaddr for each listen, for each serverblock
	4) save the unique sockaddr in serverconfig -> later used to setup listening sockets
*/


bool	ServerConfig::mf_listenDNSlookup()
{
	DNSLookupHelper		helper;
	
	getAllListeners(*this, helper);
	if (!get_AddrInfo_and_Map(helper))
		return (false);
	
	//map serverblocks to sockaddr
	for (size_t i = 0; i < m_serverBlocks.size(); ++i)
	{
		const std::set<t_listeners>&	listeners = m_serverBlocks[i].getListeners();
		for (std::set<t_listeners>::const_iterator iter = listeners.begin(); iter != listeners.end(); ++iter)
		{
			// this gets the range of all the sockaddr linked to this listener
			std::pair<DNSLookupHelper::t_listenerIter, DNSLookupHelper::t_listenerIter> range;
			range = helper.listenerToAddress.equal_range(&(*iter));

			for (DNSLookupHelper::t_listenerIter it = range.first; it != range.second; ++it)
				m_serverBlocks[i].addSockAddr((struct sockaddr*)it->second);
		}
	}
	//pass the contents of vector of u_sockaddr to serverconfig (needed by ServerManager to setup listeningsockets)
	m_allSockaddr.swap(helper.uniqueSockAddr);

	//Dns helper's destructor will call freeaddrinfo on the original lists, no problem we copied all the data

	return (true);
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



/*
void	ServerManager::mf_prepareWorkers()
{
	Nginx_MemoryPool*									workerMemPool;

	//for getaddrinfo
	std::set<const t_addrinfo*, AddrinfoPtrComparator> 	unique_Addrinfo;
	std::vector<t_addrinfo*> 							allLists_Addrinfo;

	const int backlog = 100;																								// must be replaced with correct value
	m_listenerCount = getAddrInfo_Setup(m_config, unique_Addrinfo, allLists_Addrinfo, SOCK_STREAM, AF_INET);				///must be replaced with correct values

	for (size_t i = 0; i < m_workers.size(); i++)
	{
		// create a memorypool for the worker, allocate itself in it, save pointer in ServerManager
		workerMemPool = Nginx_MemoryPool::create(4096, 1);
		m_workers[i] = (ServerWorker*)workerMemPool->allocate(sizeof (ServerWorker));
		new (m_workers[i]) ServerWorker(*this, i, *workerMemPool, m_globals);

		for (std::set<const t_addrinfo*, AddrinfoPtrComparator>::iterator iter = unique_Addrinfo.begin(); iter != unique_Addrinfo.end(); ++iter)
		{	
			m_workers[i]->createListeningSocket
			(
				**iter, 
				backlog, 
				m_protoModules[HTTP_MODULE], 
				m_initProtoConnection[HTTP_MODULE]
			);
		}
	}

	getAddrInfo_Free(allLists_Addrinfo);
}
*/