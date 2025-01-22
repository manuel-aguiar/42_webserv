
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

typedef std::set<const t_listeners*, ListenerPtrComparator> 							Set_uniqueListeners;

typedef std::pair<const t_listeners*, const BindAddress*> 								Pair_listenerToBind;
typedef std::multimap<const t_listeners*, const BindAddress*, ListenerPtrComparator> 	MMap_listenerToBind;

typedef std::pair<const t_addrinfo*, const BindAddress*> 								Pair_AddrInfoToBind;
typedef std::map<const t_addrinfo*, const BindAddress*, AddrinfoPtrComparator> 			Map_AddrInfoToBind;

struct DNSLookupHelper
{
	~DNSLookupHelper()
	{
		for (size_t i = 0; i < allAddrInfo.size(); ++i)
			::freeaddrinfo(allAddrInfo[i]);
	}

	Set_uniqueListeners			uniqueListeners; 	// unique t_listeners
	std::vector<t_addrinfo*>	allAddrInfo;		// all t_addrinfo structs we find

	MMap_listenerToBind			listenerToBind;
	Map_AddrInfoToBind			uniqueAddrInfo;

	std::vector<BindAddress>	uniqueSockAddr;
};

static bool DNSLookup(const t_listeners &listener, DNSLookupHelper& helper)
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
		Map_AddrInfoToBind::iterator addrIter = helper.uniqueAddrInfo.find(cur);

		//already in the map, associate listener with the unique address we found before
		if (addrIter != helper.uniqueAddrInfo.end())
		{
			helper.listenerToBind.insert(Pair_listenerToBind(&listener, addrIter->second));
			continue ;
		}

		//new struct based on t_addrinfo
		BindAddress address = (BindAddress){};

		//adding unique address, and length for ListeningSocket::bind()
		std::memcpy(&address.sockaddr, cur->ai_addr, cur->ai_addrlen);
		address.addrlen = cur->ai_addrlen;
		helper.uniqueSockAddr.push_back(address);

		//adding unique address to map, will should up in the next lookup
		std::pair<Map_AddrInfoToBind::iterator, bool> returnVal;
		returnVal = helper.uniqueAddrInfo.insert(Pair_AddrInfoToBind(cur, &(helper.uniqueSockAddr.back())));
		
		if (returnVal.second == false)  // not needed lol
			return (false);				
		
		// connecting listener to u_sockaddr, where we inserted the new BindAddress struct
		helper.listenerToBind.insert(Pair_listenerToBind(&listener, returnVal.first->second));
	}
	::freeaddrinfo(helper.allAddrInfo.back());
	helper.allAddrInfo.pop_back();

	return (true);
}


/*
	All in one go, to be fancy and unmaintainable.
	We loop through all the listen directives of each server block, one at a time.
	If it is new, update our uniqueListeners (avoid repitition), do the dns lookup,
	update our uniqueAddrInfo (avoid repitition), create the BindAddress struct,
	which takes sockaddr and socklen_t (needed by ServerManager::ListeningSocket::bind())

	If, on the other hand, the listener is not unique and we found it before, look
	at BindAddress* associated with this listener and add them back to the ServerBlocks.m_myListenSockAddr

	At the end, we will have a list of unique BindAddress structs, so we swap the one on the helper
	with the one on the ServerConfig (why copying..)

	We must free the addrinfo structs we copied the data from (getaddrinfo allocates these for us).
	We placed the ::freeaddrinfo in the destructor of our helper function, so if anything goes wrong
	(missconfiguration), RAII will ensure that any structs we have allocated so far will be freed
	(and at the end)

	All typedefs and helper structs placed on this .cpp file because we don't want to
	bloat the global namespace with them, not needed.
*/

bool	ServerConfig::mf_listenDNSlookup()
{
	DNSLookupHelper		helper;
	
	for (size_t i = 0; i < m_serverBlocks.size(); ++i)
	{
		const std::set<t_listeners>&			listeners = m_serverBlocks[i].getListeners();
		std::set<t_listeners>::const_iterator 	iter = listeners.begin();

		for (; iter != listeners.end(); ++iter)
		{
			std::pair<Set_uniqueListeners::iterator, bool> isUnique;
			
			// if it is new, do the DNS lookup
			isUnique = helper.uniqueListeners.insert(&(*iter));
			if (isUnique.second == true && DNSLookup(*iter, helper) == false)
			{
				//something went wrong
				return (false);
			}

			//check all BindAddress structs that are associated with this listener and add them to the block
			std::pair<MMap_listenerToBind::iterator, MMap_listenerToBind::iterator> range;
			range = helper.listenerToBind.equal_range(&(*iter));

			for (MMap_listenerToBind::iterator it = range.first; it != range.second; ++it)
				m_serverBlocks[i].addSockAddr((struct sockaddr*)it->second);
		}
	}

	m_allSockaddr.swap(helper.uniqueSockAddr);

	return (true);
}
