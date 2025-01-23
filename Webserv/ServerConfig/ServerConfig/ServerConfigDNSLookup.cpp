
# include "ServerConfig.hpp"
# include <cstddef>
# include <cstring>
# include <arpa/inet.h>
/*
	Bunch of helper structs and typedefs, purposefully placed here to avoid bloating the global namespace.
	Not needed anywhere else outside of this file.
*/

struct ListenerPtrComparator {
	bool operator()(const t_listeners* a, const t_listeners* b) const
	{
		if (a->first != b->first)
			return (a->first < b->first);
		return (a->second < b->second);
	}
};

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
		return (false);
	}
};

typedef std::pair<const t_listeners*, const t_listeners*> 								Pair_filterListeners;
typedef std::map<const t_listeners*, const t_listeners*, ListenerPtrComparator> 		Map_filterListeners;

typedef std::set<const t_addrinfo*, AddrinfoPtrComparator> 								Set_uniqueAddrInfo;

typedef std::pair<const t_listeners*, const t_addrinfo*> 								Pair_listenToAddrInfo;
typedef std::multimap<const t_listeners*, const t_addrinfo*, ListenerPtrComparator> 	MMap_listenToAddrInfo;

typedef std::pair<const t_addrinfo*, const BindAddress*> 								Pair_addrInfoToBindAddress;
typedef std::map<const t_addrinfo*, const BindAddress*, AddrinfoPtrComparator> 			Map_addrInfoToBindAddress;

struct DNSLookupHelper
{
	~DNSLookupHelper()
	{
		for (size_t i = 0; i < allAddrInfo.size(); ++i)
			::freeaddrinfo(allAddrInfo[i]);
	}

	Map_filterListeners			filterListeners; 	// a map of listeners (if two resolve to the same t_addrinfo, point to the one who resolved)
	std::vector<t_addrinfo*>	allAddrInfo;		// all t_addrinfo structs we find

	MMap_listenToAddrInfo		listenToAddrinfo;
	Set_uniqueAddrInfo			uniqueAddrInfo;

	Map_addrInfoToBindAddress	addrInfoToBindAddress;

	std::vector<BindAddress>	uniqueSockAddr;
};

static void mapEquivalentListener(DNSLookupHelper& helper, const t_listeners& dupListener, const t_addrinfo& addrinfo)
{
	if (helper.filterListeners.find(&dupListener) != helper.filterListeners.end())
		return ; // already mapped

	for (MMap_listenToAddrInfo::iterator it = helper.listenToAddrinfo.begin(); it != helper.listenToAddrinfo.end(); ++it)
	{
		if (it->second == &addrinfo)
		{
			//Map_filterListeners::iterator unique;
			//unique = helper.filterListeners.insert(Pair_filterListeners(&dupListener, it->first)).first;
			helper.filterListeners.insert(Pair_filterListeners(&dupListener, it->first)).first;
			//std::cout << dupListener.first << ":" << dupListener.second << " is now mapped to " << unique->second->first << ":" << unique->second->second <<  std::endl;	
			break ;
		}
	}
}

static void printUniqueAddrInfo(const Set_uniqueAddrInfo& uniqueAddrInfo)
{
	//std::cout << "\tuniqueAddrInfo size " << uniqueAddrInfo.size() << ":" << std::endl;
	for (Set_uniqueAddrInfo::const_iterator it = uniqueAddrInfo.begin(); it != uniqueAddrInfo.end(); ++it)
	{
		struct sockaddr_in *addr = (struct sockaddr_in*)((*it)->ai_addr);
		char ip[INET_ADDRSTRLEN];
		::inet_ntop(AF_INET, (struct sockaddr_in*)(&addr->sin_addr), ip, INET_ADDRSTRLEN);

		//std::cout << "\t\t" << ip << ":" << ::ntohs(addr->sin_port) << std::endl;
	}
}

static bool UniqueListen_UniqueAddrinfo(const t_listeners &listener, DNSLookupHelper& helper)
{
	// DNS lookup
	t_addrinfo		*res;
	t_addrinfo		*cur;
	t_addrinfo		hints;

	hints = (t_addrinfo){};
	hints.ai_family = AF_INET;    			//listener would tell me if it is ipv4 or ipv6
	hints.ai_socktype = SOCK_STREAM;		//listener would tell me if it is tcp or udp or something else

	if (::getaddrinfo(listener.first.c_str(), listener.second.c_str(), &hints, &res) != 0)
		return (false);
	helper.allAddrInfo.push_back(res);

	for (cur = res; cur != NULL; cur = cur->ai_next)
	{
		printUniqueAddrInfo(helper.uniqueAddrInfo);
		Set_uniqueAddrInfo::iterator addrIter = helper.uniqueAddrInfo.find(cur);

		//addrinfo already in map, means this listener is equivalent to another, find it and map it
		if (addrIter != helper.uniqueAddrInfo.end())
		{
			//std::cout << listener.first << ":" << listener.second << " is duplicated" << std::endl;
			mapEquivalentListener(helper, listener, **addrIter);
			continue ;
		}
		//std::cout << listener.first << ":" << listener.second << " is unique" << std::endl;

		Map_filterListeners::iterator unique = helper.filterListeners.find(&listener);
		if (unique == helper.filterListeners.end())
			unique = helper.filterListeners.insert(Pair_filterListeners(&listener, &listener)).first;
		//std::cout << listener.first << ":" << listener.second << " is now mapped to " << unique->second->first << ":" << unique->second->second << std::endl;	


		//new struct based on t_addrinfo
		


		//adding unique address to map, will should up in the next lookup
		std::pair<Set_uniqueAddrInfo::iterator, bool> returnVal;
		returnVal = helper.uniqueAddrInfo.insert(cur);

		if (returnVal.second == false)  // not needed lol
			return (false);

		struct sockaddr_in *addr = (struct sockaddr_in*)(cur->ai_addr);
		char ip[INET_ADDRSTRLEN];
		::inet_ntop(AF_INET, (struct sockaddr_in*)(&addr->sin_addr), ip, INET_ADDRSTRLEN);

		//std::cout << "added unique address: " << ip << ":" << ::ntohs(addr->sin_port) << "\n" << std::endl;

		if (returnVal.second == false)  // not needed lol
			return (false);

		// connecting listener to u_sockaddr, where we inserted the new BindAddress struct
		helper.listenToAddrinfo.insert(Pair_listenToAddrInfo(unique->second, *(returnVal.first)));
	}

	//listener is unique, map myself

	return (true);
}

/*
	BindAddress address = (BindAddress){};

	//adding unique address, and length for ListeningSocket::bind()
	std::memcpy(&address.sockaddr, cur->ai_addr, cur->ai_addrlen);
	address.addrlen = cur->ai_addrlen;

	helper.uniqueSockAddr.push_back(address);

*/

/*
	All in one go, to be fancy and unmaintainable.
	We loop through all the listen directives of each server block, one at a time.
	If it is new, update our filterListeners (avoid repitition), do the dns lookup,
	update our uniqueAddrInfo (avoid repitition), create the BindAddress struct,
	which takes sockaddr and socklen_t (needed by ServerManager::ListeningSocket::bind())

	If, on the other hand, the listener is not unique and we found it before, look
	at BindAddress* associated with this listener and add them back to the ServerBlocks.m_myListenSockAddr

	At the end, we will have a list of unique BindAddress structs, so we swap the one on the helper
	with the one on the ServerConfig (why copying..)

	We ARE NOT doing conversion of network to host byte order: sockets accepted by accept() are in network byte order
	as well, the match will be done in network order format. The lsiteningsockets are also setup
	based on network byte order, so avoids the "deconverting" step.
	Should the user want to print port and IP, they have to do THE CONVERTION TO HOST THEMSELVES VIA:

		/////IPV4 SPECIFIC PART, SHOUD BE SWITCHED IF WE ADDED IPV6////////
		struct sockaddr_in *addr = (struct sockaddr_in*)&address.sockaddr;
		addr->sin_port			= ::ntohs(addr->sin_port);
		addr->sin_addr.s_addr 	= ::ntohl(addr->sin_addr.s_addr);
		///////////////////////////////////////////////////////////////////	


	We must free the addrinfo structs we copied the data from (getaddrinfo allocates these for us).
	We placed the ::freeaddrinfo in the destructor of our helper function, so if anything goes wrong
	(missconfiguration), RAII will ensure that any structs we have allocated so far will be freed
	(and at the end)
	
	All typedefs and helper structs placed on this .cpp file because we don't want to
	bloat the global namespace with them, not needed.
*/

static void Setup_Addrinfo_To_BindAddress(DNSLookupHelper&		helper)
{
	//std::cout << "unique address: " << helper.uniqueAddrInfo.size() << std::endl;

	Set_uniqueAddrInfo::iterator it = helper.uniqueAddrInfo.begin();

	for (; it != helper.uniqueAddrInfo.end(); ++it)
	{
		BindAddress address = (BindAddress){};

		//adding unique address, and length for ListeningSocket::bind()
		std::memcpy(&address.sockaddr, (*it)->ai_addr, (*it)->ai_addrlen);
		address.addrlen = (*it)->ai_addrlen;

		helper.uniqueSockAddr.push_back(address);
	}

	it = helper.uniqueAddrInfo.begin();
	size_t i = 0;

	for (; it != helper.uniqueAddrInfo.end() && i < helper.uniqueSockAddr.size(); ++it, ++i)
	{
		helper.addrInfoToBindAddress.insert(Pair_addrInfoToBindAddress(*it, &helper.uniqueSockAddr[i]));
	}

}

bool	ServerConfig::mf_listenDNSlookup()
{
	DNSLookupHelper		helper;
	
	// do dns lookup, mappings
	for (size_t i = 0; i < m_serverBlocks.size(); ++i)
	{
		const std::set<t_listeners>&			listeners = m_serverBlocks[i].getListeners();
		std::set<t_listeners>::const_iterator 	iter = listeners.begin();

		for (; iter != listeners.end(); ++iter)
		{
			// if it is new, do the DNS lookup
			if (helper.filterListeners.find(&(*iter)) == helper.filterListeners.end() 	// not mapped to unique
			&& UniqueListen_UniqueAddrinfo(*iter, helper) == false)						// failed to lookup
			{
				//something went wrong
				return (false);
			}
		}
	}

	Setup_Addrinfo_To_BindAddress(helper);

	// fill server blocks with their listeners
	for (size_t i = 0; i < m_serverBlocks.size(); ++i)
	{
		const std::set<t_listeners>&			listeners = m_serverBlocks[i].getListeners();
		std::set<t_listeners>::const_iterator 	iter = listeners.begin();
		std::set<const t_listeners*>			myListeners;
		for (; iter != listeners.end(); ++iter)
		{
			//check all BindAddress structs that are associated with this listener and add them to the block

			const t_listeners* target = helper.filterListeners.find(&(*iter))->second;
			if (myListeners.insert(target).second == false)
				continue ;

			std::pair<MMap_listenToAddrInfo::iterator, MMap_listenToAddrInfo::iterator> range;
			range = helper.listenToAddrinfo.equal_range(target);

			for (MMap_listenToAddrInfo::iterator it = range.first; it != range.second; ++it)
			{

				Map_addrInfoToBindAddress::iterator addrIter = helper.addrInfoToBindAddress.find(it->second);

				//struct sockaddr_in *addr = (struct sockaddr_in*)(&addrIter->second->sockaddr);
				//char ip[INET_ADDRSTRLEN];
				//::inet_ntop(AF_INET, (struct sockaddr_in*)(&addr->sin_addr), ip, INET_ADDRSTRLEN);

				m_serverBlocks[i].addListenSockAddr((struct sockaddr*)(&addrIter->second->sockaddr));

				//std::cout << "address " << ip << ":" << ::ntohs(addr->sin_port) << " added to server block " << i
				//<< "from listener " << it->first->first << ":" << it->first->second << "\n" << std::endl;
			}
		}
	}

	m_allSockaddr.swap(helper.uniqueSockAddr);

	return (true);
}
