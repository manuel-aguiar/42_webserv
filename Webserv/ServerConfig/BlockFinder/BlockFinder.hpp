#ifndef BLOCKFINDER_HPP
# define BLOCKFINDER_HPP

// our headers
# include "../../Ws_Namespace.h"

// C++ headers
# include <string>
# include <map>
# include <set>
# include <iostream>
# include <iomanip>
# include <vector>

# include "../../../Toolkit/MemoryPool/Heap_ObjectPool/Heap_ObjectPool.hpp"
# include "../../GenericUtils/BufferView/BufferView.hpp"

class ServerBlock;

class BlockFinder
{
	public:
		// # of entries = # listeners (defaults) + sum serverblock(this block names * this block listeners)
		BlockFinder(const size_t numberOfEntries);
		~BlockFinder();

		void				loadServerBlocks(const std::vector<ServerBlock>& blocks);
		void				addServerBlock(const ServerBlock& block);
		const ServerBlock*	findServerBlock(const Ws::Sock::addr& address, const std::string& serverName);
		bool				hasServerBlock(const Ws::Sock::addr& address, const std::string& serverName);
		void				removeServerBlock(const Ws::Sock::addr& address, const std::string& serverName);

	private:
		struct BlockIdentifier
		{
			BlockIdentifier();
			BlockIdentifier(const Ws::Sock::addr& addr, const BufferView serverName);
			~BlockIdentifier();
			BlockIdentifier(const BlockIdentifier& copy);
			
			const Ws::Sock::addr* 	sockaddr;
			const BufferView		serverName;	
			
			// comparison operator
			bool operator<(const BlockIdentifier& other) const;
			
			private:
				BlockIdentifier& operator=(const BlockIdentifier& copy);
		};

		typedef Heap_ObjectPool<BlockIdentifier>					mapPool;
		typedef std::map<const BlockIdentifier, const ServerBlock*, std::less<BlockIdentifier>, mapPool> 					
																	blockIDs;
		blockIDs						m_serverBlocks;
};

#endif
