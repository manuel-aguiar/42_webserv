#ifndef BLOCKFINDER_HPP
# define BLOCKFINDER_HPP

// our headers
# include "../../Ws_Namespace.h"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"


// C++ headers
# include <string>
# include <map>
# include <set>
# include <iostream>
# include <iomanip>

class ServerBlock;

class BlockFinder {
	public:
		BlockFinder();
		~BlockFinder();

		void				loadServerBlocks(const std::vector<ServerBlock>& blocks);
		void				addServerBlock(const ServerBlock& block);
		const ServerBlock*	findServerBlock(Ws::Sock::addr* address, const std::string& serverName);
		bool				hasServerBlock(Ws::Sock::addr* address, const std::string& serverName);
		void				removeServerBlock(Ws::Sock::addr* address, const std::string& serverName);

	private:
		struct BlockIdentifier {
			uint32_t    ip;
			uint16_t    port;
			std::string serverName;

			// comparison operator
			bool operator<(const BlockIdentifier& other) const;
		};

		BlockIdentifier                              		m_wildcardKey;
		std::map<BlockIdentifier, const ServerBlock*>		m_serverBlocks;

		std::pair<uint32_t, uint16_t>	mf_extractIpPort(const Ws::Sock::addr* addr) const;
		BlockIdentifier				 	 mf_createIdentifier(const Ws::Sock::addr* addr, const std::string& serverName) const;
};

#endif
