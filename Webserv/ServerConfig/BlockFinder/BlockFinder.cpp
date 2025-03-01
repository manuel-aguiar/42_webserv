

#include "BlockFinder.hpp"

#include "../ServerBlock/ServerBlock.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

BlockFinder::BlockFinder(const size_t numberOfEntries) :
	m_serverBlocks(std::less<BlockIdentifier>(), mapPool(numberOfEntries))
{}

BlockFinder::~BlockFinder() {}

/*
**	hasServerBlock
**	checks if a server block with the given ip, port and server name exists
**	uses normalized directives
*/
bool	BlockFinder::hasServerBlock(const Ws::Sock::addr& address, const std::string& serverName)
{
	return (findServerBlock(address, serverName) != NULL);
}

/*
**	addServerBlock
**	adds a server block to the server_blocks map for each unique combination of listeners and server names
**	blocks are added with normalized directives
**  if a directive is not provided, falls back to the wildcard value
**	if the block is already in the map, it is not added again
*/
void	BlockFinder::addServerBlock(const ServerBlock& block)
{
	const std::vector<const Ws::Sock::addr*>& listeners = block.getListenAddresses();
	const std::set<std::string>& serverNames = block.getServerNames();

	for (std::vector<const Ws::Sock::addr*>::const_iterator lit = listeners.begin(); lit != listeners.end(); ++lit)
	{
		for (std::set<std::string>::const_iterator sit = serverNames.begin(); sit != serverNames.end(); ++sit)
		{
			ASSERT_EQUAL((*lit)->sa_family, AF_INET, 
					"BlockFinder::addServerBlock(): Currently supports only ipv4");

			BlockIdentifier blockKey(**lit, BufferView(*sit));
			if (m_serverBlocks.find(blockKey) == m_serverBlocks.end())
				m_serverBlocks[blockKey] = &block;
			
			BlockIdentifier defaultKey(**lit, BufferView());
			if (m_serverBlocks.find(defaultKey) == m_serverBlocks.end())
				m_serverBlocks[defaultKey] = &block;
		}
	}
}

void    BlockFinder::loadServerBlocks(const std::vector<ServerBlock>& blocks)
{
	for (std::vector<ServerBlock>::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
		addServerBlock(*it);
}

/*
**	findServerBlock
**	Finds the server block that matches the given IP, port, and server name.
**	Returns a pointer to the server block if found, otherwise returns NULL.
**
**	The order of precedence for finding a matching block is as follows:
**	- Exact match for the combination of IP, port, and server name.
**	- IP:port match, but wildcard (*) server name.
**	- IP wildcard (0.0.0.0), but exact port and server name.
*/
const ServerBlock*	BlockFinder::findServerBlock(const Ws::Sock::addr& address, const std::string& serverName)
{
	#ifndef NDEBUG

		blockIDs::const_iterator assertIt = m_serverBlocks.begin();
		for (; assertIt != m_serverBlocks.end(); ++assertIt)
		{
			const Ws::Sock::addr_in* addr = (const Ws::Sock::addr_in*)assertIt->first.sockaddr;
			if (::memcmp(addr, &address, sizeof(Ws::Sock::addr_in)) == 0)
				break ;
		}

		ASSERT_EQUAL(assertIt != m_serverBlocks.end(), true, 
				"BlockFinder::findServerBlock(): the address passed must be of a listeningSocket");
	#endif


	BlockIdentifier 			blockKey(address, BufferView(serverName));
	blockIDs::const_iterator 	it = m_serverBlocks.find(blockKey);

	if (it != m_serverBlocks.end())
		return (it->second);
	
	BlockIdentifier defaultKey(address, BufferView());
	it = m_serverBlocks.find(defaultKey);

	ASSERT_EQUAL(it != m_serverBlocks.end(), true, 
			"BlockFinder::findServerBlock(): there must be a default server available");

	return (it->second);
}

/*
**	removeServerBlock
**	removes a server block from the server_blocks map
*/
void	BlockFinder::removeServerBlock(const Ws::Sock::addr& address, const std::string& serverName)
{
	BlockIdentifier 			blockKey(address, BufferView(serverName));
	blockIDs::iterator 	blockIt = m_serverBlocks.find(blockKey);

	if (blockIt != m_serverBlocks.end())
		m_serverBlocks.erase(blockIt);
	

	// if target is the default server as well, we have to remove it
	BlockIdentifier 			defaultKey(address, BufferView());
	blockIDs::iterator 			defaultIt = m_serverBlocks.find(defaultKey);

	ASSERT_EQUAL(defaultIt != m_serverBlocks.end(), true, 
			"BlockFinder::findServerBlock(): No default block found for ip:port");

	if (defaultIt->second == blockIt->second)
		m_serverBlocks.erase(defaultIt);

}



//since i am using c++98 for the server, it could be something like

BlockFinder::BlockIdentifier::BlockIdentifier() : sockaddr(NULL), serverName(NULL) {}
BlockFinder::BlockIdentifier::BlockIdentifier(const Ws::Sock::addr& addr, const BufferView serverName) 
	: sockaddr(&addr)
	, serverName(serverName)
{}

BlockFinder::BlockIdentifier::~BlockIdentifier() {}
BlockFinder::BlockIdentifier::BlockIdentifier(const BlockIdentifier& copy) 
	: sockaddr(copy.sockaddr)
	, serverName(copy.serverName)
{}


bool
BlockFinder::BlockIdentifier::operator<(const BlockIdentifier& other) const 
{
	// asserts for only ipv4, would require a switch for ipv6
	ASSERT_EQUAL(sockaddr != NULL, true, 
			"BlockFinder::BlockIdentifier::operator<(): sockaddr is NULL");
	ASSERT_EQUAL(other.sockaddr != NULL, true, 
			"BlockFinder::BlockIdentifier::operator<(): other.sockaddr is NULL");
	ASSERT_EQUAL(sockaddr->sa_family, AF_INET, 
			"BlockFinder::BlockIdentifier::operator<(): Currently supports only ipv4");
	ASSERT_EQUAL(other.sockaddr->sa_family, AF_INET, 
			"BlockFinder::BlockIdentifier::operator<(): Currently supports only ipv4");
	
	int compare = ::memcmp(	((Ws::Sock::addr_in*)sockaddr), 
							((Ws::Sock::addr_in*)other.sockaddr), 
							sizeof(Ws::Sock::addr_in));
	
	if (compare != 0)
		return (compare < 0);
	return (serverName < other.serverName);
	
	
}

// private
BlockFinder::BlockIdentifier&
BlockFinder::BlockIdentifier::operator=(const BlockIdentifier& copy)
{
	if (this == &copy)
		return (*this);
	sockaddr = copy.sockaddr;
	return (*this);
}