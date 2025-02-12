
#ifndef SERVERBLOCK_HPP

# define SERVERBLOCK_HPP

// C++ headers
# include <iostream>
# include <string>
# include <map>
# include <set>
# include <vector>
# include <exception>

// Own headers

# include "../../Ws_Namespace.h"

//forward declarations
class ServerLocation;
class DefaultConfig;

//necessary structs
namespace Config
{
	struct Listen
	{
		Ws::AppLayer::Type	appLayer;
		Ws::strIP			hostname;
		Ws::strPort			port;

		bool operator<(const Listen &rhs) const;
	};
}

class ServerBlock
{
	public:
		ServerBlock();
		~ServerBlock();
		ServerBlock &operator=(const ServerBlock &other);
		ServerBlock(const ServerBlock &other);

		const std::set<std::string>&						getDomainNames() const;
		const std::map<std::string, ServerLocation>& 		getLocations() const;

		void							setLocations(const std::vector<ServerLocation> &Locations);
		void							setRootPath(const std::string &value);
		void							setClientBodySize(const std::string &value);
		void							setClientHeaderSize(const std::string &value);
		void							addListener(const std::string &value);
		void							addServerName(const std::string &value);
		void							addErrorPage(const std::string &value);
		const std::set<Config::Listen>&	getListeners() const;
		const std::set<std::string>&	getServerNames() const;
		size_t							getClientBodySize() const;
		size_t							getClientHeaderSize() const;
		const std::set<std::string>&	getErrorPages() const;
		const std::string&				getRoot() const;

		void							setDefaults(const DefaultConfig& defaultConfig);
		void							addConfigValue(const std::string &key, const std::string &value);
		bool							validate() const;

		// Debug
		void							printServerConfig() const;

		void							addListenAddress(const struct sockaddr* addr);
		const std::vector<const struct sockaddr*>&	
										getListenAddresses() const;

	private:

		typedef void (ServerBlock::*f_addConfigValue)(const std::string &);
		std::map<std::string, f_addConfigValue> 		m_keys;

		std::set<Config::Listen>						m_listen;
		std::set<std::string>							m_server_name;
		std::string										m_client_body_size;
		std::string										m_client_header_size;
		std::string										m_root;
		std::set<std::string>							m_error_pages;

		std::map<Ws::path, ServerLocation>				m_locations;

		std::vector<const Ws::Sock::addr*>				m_myListenAddresses;
};

#endif
