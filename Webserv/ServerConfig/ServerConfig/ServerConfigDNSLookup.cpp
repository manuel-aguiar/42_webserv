
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

typedef std::pair<const t_listeners*, const t_listeners*> 								Pair_toUniqueListener;
typedef std::map<const t_listeners*, const t_listeners*, ListenerPtrComparator> 		Map_toUniqueListener;

typedef std::set<const t_addrinfo*, AddrinfoPtrComparator> 								Set_uniqueAddr;

typedef std::pair<const t_listeners*, const t_addrinfo*> 								Pair_uniqueListenToAddr;
typedef std::multimap<const t_listeners*, const t_addrinfo*, ListenerPtrComparator> 	MMap_uniqueListenToAddr;

typedef std::pair<const t_addrinfo*, const BindAddress*> 								Pair_uniqueAddrToBind;
typedef std::map<const t_addrinfo*, const BindAddress*, AddrinfoPtrComparator> 			Map_uniqueAddrToBind;

struct DNSLookupHelper
{
	~DNSLookupHelper()
	{
		for (size_t i = 0; i < allAddr.size(); ++i)
			::freeaddrinfo(allAddr[i]);
	}
	std::vector<t_addrinfo*>	allAddr;		// all t_addrinfo structs we find

	Map_toUniqueListener		listenerToUniqueListener; 	// a map of listeners (if two resolve to the same t_addrinfo, point to the one who resolved)

	MMap_uniqueListenToAddr		uniqueListenToAddr;
	Set_uniqueAddr				uniqueAddr;

	Map_uniqueAddrToBind		uniqueAddrToBind;

	std::vector<BindAddress>	uniqueBind;
};

/*
	Find out who is the unique listener that setup that addrinfo, originally, and map us to that listener
*/
static void mapEquivalentListener(DNSLookupHelper& helper, const t_listeners& dupListener, const t_addrinfo& targetAddrinfo)
{
	if (helper.listenerToUniqueListener.find(&dupListener) != helper.listenerToUniqueListener.end())
		return ; // already mapped

	for (MMap_uniqueListenToAddr::iterator it = helper.uniqueListenToAddr.begin(); it != helper.uniqueListenToAddr.end(); ++it)
	{
		if (it->second == &targetAddrinfo)
		{
			helper.listenerToUniqueListener.insert(Pair_toUniqueListener(&dupListener, it->first)).first;
			break ;
		}
	}
}

/*
	Does the DNS lookup for all the listeners in the server block.

	As more server blocks are rolled in, listeners and sockaddrinfo's are filtered and the end result is:

	Set_uniqueAddr -> a set of unique addrinfo structs
	mapToUniqueListener -> a map that lists all listeners to their unique counterpart

*/

static bool Map_Listener_and_Addrinfo(const t_listeners &listener, DNSLookupHelper& helper)
{
	// DNS lookup
	t_addrinfo		*res;
	t_addrinfo		*cur;
	t_addrinfo		hints;

	hints = (t_addrinfo){};
	hints.ai_family = AF_INET;    			//listener would tell me if it is ipv4 or ipv6
	hints.ai_socktype = SOCK_STREAM;		//listener would tell me if it is tcp or udp or something else

	if (::getaddrinfo(listener.first.c_str(), listener.second.c_str(), &hints, &res) != 0)
	{
		std::cerr << "Error: DNS lookup failed for " << listener.first << ":" << listener.second << std::endl;
		return (false);
	}
		
	helper.allAddr.push_back(res);

	for (cur = res; cur != NULL; cur = cur->ai_next)
	{
		Set_uniqueAddr::iterator addrIter = helper.uniqueAddr.find(cur);

		if (addrIter != helper.uniqueAddr.end())
		{
			//std::cout << listener.first << ":" << listener.second << " is duplicated" << std::endl;
			mapEquivalentListener(helper, listener, **addrIter);
			continue ;
		}

		Map_toUniqueListener::iterator unique = helper.listenerToUniqueListener.find(&listener);
		if (unique == helper.listenerToUniqueListener.end())
			unique = helper.listenerToUniqueListener.insert(Pair_toUniqueListener(&listener, &listener)).first;

		std::pair<Set_uniqueAddr::iterator, bool> returnVal;
		returnVal = helper.uniqueAddr.insert(cur);
		helper.uniqueListenToAddr.insert(Pair_uniqueListenToAddr(unique->second, *(returnVal.first)));
	}

	return (true);
}

/*
	Now here we have the unique addresses and listeners map.

	We first setup the vector of BindAddress built from the unique addrinfos, to store on the ServerConfig.
	Then we create a map of addrinfo to the location where the bindAddress was built, so at
	the final stage we can link listeners to their final destination BindAddress
*/
static void Map_Addrinfo_To_BindAddress(DNSLookupHelper&	helper)
{
	// nowe we can do the mapping since we know how many Set_uniqueAddr we have
	helper.uniqueBind.reserve(helper.uniqueAddr.size());

	Set_uniqueAddr::iterator it = helper.uniqueAddr.begin();

	for (; it != helper.uniqueAddr.end(); ++it)
	{
		BindAddress address = (BindAddress){};

		std::memcpy(&address.address, (*it)->ai_addr, (*it)->ai_addrlen);
		address.addrlen = (*it)->ai_addrlen;
		address.addrFamily = (*it)->ai_family;
		address.socktype = (*it)->ai_socktype;
		address.protocol = (*it)->ai_protocol;
		helper.uniqueBind.push_back(address);
		helper.uniqueAddrToBind.insert(Pair_uniqueAddrToBind(*it, &helper.uniqueBind.back()));
	}
}

/*
	Go through every serverblock, check every listener, if it is not mapped yet
	to their unique counterpart, perform a DNSLookup and map the listener to the unique addrinfo
	(either itself or another listener whose addrinfo is the same)
*/
static bool DNSLookup_MapUniques(const ServerConfig& config, DNSLookupHelper&		helper)
{
	
	const std::vector<ServerBlock>& serverBlocks = config.getServerBlocks();

	for (size_t i = 0; i < serverBlocks.size(); ++i)
	{
		const std::set<t_listeners>&			listeners = serverBlocks[i].getListeners();
		std::set<t_listeners>::const_iterator 	iter = listeners.begin();

		for (; iter != listeners.end(); ++iter)
		{
			if (helper.listenerToUniqueListener.find(&(*iter)) == helper.listenerToUniqueListener.end() 	// not mapped to unique
			&& Map_Listener_and_Addrinfo(*iter, helper) == false)											// failed to lookup
			{
				return (false);
			}
		}
	}
	return (true);
}


/*

	Function divided into 4 stages:

	1. Do the DNS lookup for all the listeners in all server blocks
		a) as we go, we build a set of unique addrinfo structs
		b) we build a map of ALL listeners to their UNIQUE counterpart (the first that resolved to that addrinfo)
			(since two exact same addrinfo structs imply the same BindAddress, hence localhost being equal 127.0.0.1, same address)
	2. Create a vector (uniqueBind) of BindAddress structs, one for each unique addrinfo struct
		a) As we go, we also map the unique addrinfo to their BindAddress structs (uniqueBind)
	3. Fill the server blocks with their listeners, by mapping the listeners to their addrinfo and then to their BindAddress structs
		a) important step, each block may have duplicates themselves
			so we need to keep track, at each block, their own listener duplicates to avoid having the same input twice.
	4. Swap the uniqueBindAddress with the one on the ServerConfig, no need to copy
*/
bool	ServerConfig::mf_listenDNSlookup()
{
	DNSLookupHelper		helper;
	
	//1)
	if (DNSLookup_MapUniques(*this, helper) == false)
		return (false);

	//2)
	Map_Addrinfo_To_BindAddress(helper);

	//3)
	for (size_t i = 0; i < m_serverBlocks.size(); ++i)
	{
		const std::set<t_listeners>&			listeners = m_serverBlocks[i].getListeners();
		std::set<t_listeners>::const_iterator 	iter = listeners.begin();

		//track own duplicates
		std::set<const t_listeners*>			myFiltered;	
		for (; iter != listeners.end(); ++iter)
		{
			// check if myself have a doubled listener. if i do, skip and let the unique fill the sockaddr
			const t_listeners* target = helper.listenerToUniqueListener.find(&(*iter))->second;
			if (myFiltered.insert(target).second == false)
				continue ;

			// get all the addrinfo structs associated with the UNIQUE listener
			std::pair<MMap_uniqueListenToAddr::iterator, MMap_uniqueListenToAddr::iterator> range;
			range = helper.uniqueListenToAddr.equal_range(target);

			// find the relevant BindAddress struct in the ServerConfig vector
			for (MMap_uniqueListenToAddr::iterator it = range.first; it != range.second; ++it)
			{
				Map_uniqueAddrToBind::iterator addrIter = helper.uniqueAddrToBind.find(it->second);
				m_serverBlocks[i].addListenAddress((struct sockaddr*)(&addrIter->second->address));
			}
		}
	}

	//4)
	m_bindAddresses.swap(helper.uniqueBind);

	return (true);
}
