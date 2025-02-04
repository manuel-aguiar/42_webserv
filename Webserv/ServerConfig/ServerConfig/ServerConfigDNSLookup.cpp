


# include "ServerConfig.hpp"
# include "../ServerBlock/ServerBlock.hpp"

# include <cstddef>
# include <cstring>
# include <arpa/inet.h>
/*
	Bunch of helper structs and typedefs, purposefully placed here to avoid bloating the global namespace.
	Not needed anywhere else outside of this file.
*/

struct ListenerPtrComparator {
	bool operator()(const Config::Listen* a, const Config::Listen* b) const
	{
		if (a->appLayer != b->appLayer)
			return (a->appLayer < b->appLayer);
		if (a->hostname != b->hostname)
			return (a->hostname < b->hostname);
		return (a->port < b->port);
	}
};

struct AddrinfoPtrComparator
{
	bool operator()(const Ws::Sock::Info* a, const Ws::Sock::Info* b)
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

typedef std::pair<const Config::Listen*, const Config::Listen*> 								Pair_toUniqueListener;
typedef std::map<const Config::Listen*, const Config::Listen*, ListenerPtrComparator> 		Map_toUniqueListener;

typedef std::set<const Ws::Sock::Info*, AddrinfoPtrComparator> 								Set_uniqueAddr;

typedef std::pair<const Config::Listen*, const Ws::Sock::Info*> 								Pair_uniqueListenToAddr;
typedef std::multimap<const Config::Listen*, const Ws::Sock::Info*, ListenerPtrComparator> 	MMap_uniqueListenToAddr;

typedef std::pair<const Ws::Sock::Info*, const Ws::BindInfo*> 								Pair_uniqueAddrToBind;
typedef std::map<const Ws::Sock::Info*, const Ws::BindInfo*, AddrinfoPtrComparator> 			Map_uniqueAddrToBind;

struct DNSLookupHelper
{
	~DNSLookupHelper()
	{
		for (size_t i = 0; i < allAddr.size(); ++i)
			::freeaddrinfo(allAddr[i]);
	}
	std::vector<Ws::Sock::Info*>	allAddr;		// all Ws::Sock::Info structs we find

	Map_toUniqueListener		listenerToUniqueListener; 	// a map of listeners (if two resolve to the same Ws::Sock::Info, point to the one who resolved)

	MMap_uniqueListenToAddr		uniqueListenToAddr;
	Set_uniqueAddr				uniqueAddr;

	Map_uniqueAddrToBind		uniqueAddrToBind;

	std::vector<Ws::BindInfo>	uniqueBind;
};

/*
	Find out who is the unique listener that setup that addrinfo, originally, and map us to that listener
*/
static const Config::Listen& mapEquivalentListener(DNSLookupHelper& helper, const Config::Listen& dupListener, const Ws::Sock::Info& targetAddrinfo)
{
	Map_toUniqueListener::iterator unique = helper.listenerToUniqueListener.find(&dupListener);

	if (unique != helper.listenerToUniqueListener.end())
		return (*unique->second); // already mapped

	for (MMap_uniqueListenToAddr::iterator it = helper.uniqueListenToAddr.begin(); it != helper.uniqueListenToAddr.end(); ++it)
	{
		if (it->second == &targetAddrinfo)
		{
			std::pair<Map_toUniqueListener::iterator, bool> returnVal;
			returnVal = helper.listenerToUniqueListener.insert(Pair_toUniqueListener(&dupListener, it->first));
			unique = returnVal.first;
		}
	}
	return (*unique->second);
}

/*
	Does the DNS lookup for all the listeners in the server block.

	As more server blocks are rolled in, listeners and sockaddrinfo's are filtered and the end result is:

	Set_uniqueAddr -> a set of unique addrinfo structs
	mapToUniqueListener -> a map that lists all listeners to their unique counterpart

*/

static bool Map_Listener_and_Addrinfo(const Config::Listen& listener, DNSLookupHelper& helper)
{
	// DNS lookup
	Ws::Sock::Info		*res;
	Ws::Sock::Info		*cur;
	Ws::Sock::Info		hints;

	hints = (Ws::Sock::Info){};
	hints.ai_family = AF_INET;    			//listener would tell me if it is ipv4 or ipv6
	hints.ai_socktype = SOCK_STREAM;		//listener would tell me if it is tcp or udp or something else

	if (::getaddrinfo(listener.hostname.c_str(), listener.port.c_str(), &hints, &res) != 0)
	{
		std::cerr << "Error: DNS lookup failed for " << listener.hostname << ":" << listener.port << std::endl;
		return (false);
	}
		
	helper.allAddr.push_back(res);

	for (cur = res; cur != NULL; cur = cur->ai_next)
	{
		Set_uniqueAddr::iterator addrIter = helper.uniqueAddr.find(cur);

		if (addrIter != helper.uniqueAddr.end())
		{
			const Config::Listen& equivalent = mapEquivalentListener(helper, listener, **addrIter);
			if (listener.appLayer != equivalent.appLayer)
			{
				std::cerr << "Error: " << listener.hostname << ":" << listener.port << " is being used for more than one protocol" << std::endl;
				return (false);
			}
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
		Ws::BindInfo info = (Ws::BindInfo){};

		std::memcpy(&info.addr, (*it)->ai_addr, (*it)->ai_addrlen);
		info.appLayer = Ws::AppLayer::HTTP;
		info.addrlen = (*it)->ai_addrlen;
		info.family = (*it)->ai_family;
		info.socktype = (*it)->ai_socktype;
		info.proto = (*it)->ai_protocol;
		helper.uniqueBind.push_back(info);
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
		const std::set<Config::Listen>&			listeners = serverBlocks[i].getListeners();
		std::set<Config::Listen>::const_iterator 	iter = listeners.begin();

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
		const std::set<Config::Listen>&			listeners = m_serverBlocks[i].getListeners();
		std::set<Config::Listen>::const_iterator 	iter = listeners.begin();

		//track own duplicates
		std::set<const Config::Listen*>			myFiltered;	
		for (; iter != listeners.end(); ++iter)
		{
			// check if myself have a doubled listener. if i do, skip and let the unique fill the sockaddr
			const Config::Listen* target = helper.listenerToUniqueListener.find(&(*iter))->second;
			if (myFiltered.insert(target).second == false)
				continue ;

			// get all the addrinfo structs associated with the UNIQUE listener
			std::pair<MMap_uniqueListenToAddr::iterator, MMap_uniqueListenToAddr::iterator> range;
			range = helper.uniqueListenToAddr.equal_range(target);

			// find the relevant BindAddress struct in the ServerConfig vector
			for (MMap_uniqueListenToAddr::iterator it = range.first; it != range.second; ++it)
			{
				Map_uniqueAddrToBind::iterator addrIter = helper.uniqueAddrToBind.find(it->second);
				m_serverBlocks[i].addListenAddress((Ws::Sock::addr*)(&addrIter->second->addr));
			}
		}
	}

	//4)
	m_bindAddresses.swap(helper.uniqueBind);

	return (true);
}
