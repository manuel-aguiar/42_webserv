
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

# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../GenericUtils/Validation/Validation.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../ServerLocation/ServerLocation.hpp"
# include "../DefaultConfig/DefaultConfig.hpp"
class ServerLocation;

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
		const std::set<t_listeners>&	getListeners() const;
		const std::set<std::string>&	getServerNames() const;
		size_t							getClientBodySize() const;
		size_t							getClientHeaderSize() const;
		const std::set<std::string>&	getErrorPages() const;
		const std::string&				getRoot() const;

		void							setDefaults();
		void							addConfigValue(const std::string &key, const std::string &value);
		bool							validate() const;

		// Debug
		void							printServerConfig() const;

		void							addSockAddr(const struct sockaddr* addr);
		std::vector<const struct sockaddr*>	
										getSockAddr() const;

	private:
		typedef void (ServerBlock::*f_addConfigValue)(const std::string &);
		std::map<std::string, f_addConfigValue> 		m_keys;

		std::set<t_listeners>							m_listen;
		std::set<std::string>							m_server_name;
		std::string										m_client_body_size;
		std::string										m_client_header_size;
		std::string										m_root;
		std::set<std::string>							m_error_pages;

		std::map<t_path, ServerLocation>				m_locations;

		std::vector<const struct sockaddr*>				m_myListenAddresses;
};

#endif
