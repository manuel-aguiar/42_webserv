#ifndef BLOCKFINDER_HPP
# define BLOCKFINDER_HPP

// our headers
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

// C++ headers
# include <string>
# include <map>
# include <set>
# include <iostream>
# include <iomanip>

// forward declarations
class ServerBlock;

class BlockFinder {
	public:
		BlockFinder();
		~BlockFinder();

		void				addServerBlock(const ServerBlock& block);
		const ServerBlock*	findServerBlock(struct sockaddr* address, const std::string& serverName);
		bool				hasServerBlock(struct sockaddr* address, const std::string& serverName);
		void				removeServerBlock(struct sockaddr* address, const std::string& serverName);

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

		std::pair<uint32_t, uint16_t>	mf_extractIpPort(const struct sockaddr* addr) const;
		BlockIdentifier				 	 mf_createIdentifier(const struct sockaddr* addr, const std::string& serverName) const;
};

#endif
