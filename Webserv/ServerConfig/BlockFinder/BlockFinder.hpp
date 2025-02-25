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

class ServerBlock;

class BlockFinder {
	public:
		BlockFinder();
		~BlockFinder();

		void				loadServerBlocks(const std::vector<ServerBlock>& blocks);
		void				addServerBlock(const ServerBlock& block);
		const ServerBlock*	findServerBlock(const std::string& serverName);
		bool				hasServerBlock(const std::string& serverName);
		void				removeServerBlock(const std::string& serverName);
		void				removeServerName(const std::string& serverName);
		
	private:

		std::map<std::string, const ServerBlock*>	m_serverBlocks;
};

#endif
