
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

// for the KeySetters
# include "../../../Toolkit/MemoryPool/Heap_ObjectPool/Heap_ObjectPool.hpp"

//forward declarations
class ServerConfig;
class ServerLocation;
struct DefaultConfig;

//necessary structs
namespace Config
{
	struct Listen
	{
		Ws::AppLayer::Type		appLayer;
		Ws::Listen::backlog		backlog;
		Ws::strIP				hostname;
		Ws::strPort				port;
		Ws::Sock::addrFamily 	family;
		Ws::Sock::type			socktype;
		Ws::Sock::protocol		proto;
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

		const std::set<std::string>&	
										getDomainNames() const;
		const std::vector<ServerLocation>&
										getLocations() const;
		const std::map<std::string, const ServerLocation*>& 		
										getMappedLocations() const;

		const std::set<Config::Listen>&	getListeners() const;
		const std::set<std::string>&	getServerNames() const;
		size_t							getClientBodySize() const;
		size_t							getClientHeaderSize() const;
		int								getTimeoutFullHeader() const;
		int								getTimeoutInterSend() const;
		int								getTimeoutInterReceive() const;
		const std::set<std::string>&	getErrorPages() const;
		const std::string&				getRoot() const;
		const std::vector<const struct sockaddr*>&	
										getListenAddresses() const;


		void							setRootPath(const std::string &value);
		void							setClientBodySize(const std::string &value);
		void							setClientHeaderSize(const std::string &value);
		void							setTimeoutFullHeader(const std::string &value);
		void							setTimeoutInterSend(const std::string &value);
		void							setTimeoutInterReceive(const std::string &value);
		void							setDefaults(const DefaultConfig& defaultConfig);
		void							addListener(const std::string &value);
		void							addServerName(const std::string &value);
		void							addErrorPage(const std::string &value);
		void							addListenAddress(const struct sockaddr* addr);

		bool							fillInheritedSettings(const ServerConfig& config);

		void							addConfigValue(const std::string &key, const std::string &value);
		bool							validate() const;
		void							mapLocations();

		// Debug
		void							printServerConfig() const;

		std::vector<ServerLocation>&	accessLocations();
		std::map<std::string, const ServerLocation*>&
										accessMappedLocations();

	private:

		struct DirectiveToSetter
		{
			DirectiveToSetter();
			typedef void 															(ServerBlock::*Setter)(const std::string &);
			typedef std::pair<const std::string, Setter> 							DirectiveSetterPair;
			typedef Heap_ObjectPool<DirectiveSetterPair>							mapPool;
			typedef std::map<std::string, Setter, std::less<std::string>, mapPool> 	DirectiveSetterMap;

			DirectiveSetterMap	map;
		};

		// common to all instances
		static DirectiveToSetter						m_directiveToSetter;

		std::set<Config::Listen>						m_listen;
		std::set<std::string>							m_server_name;
		size_t											m_http_maxClientBodySize;
		size_t											m_http_maxClientHeaderSize;
		int												m_http_timeoutFullHeader;
		int												m_http_timeoutInterSend;
		int												m_http_timeoutInterReceive;
		std::string										m_root;
		std::set<std::string>							m_error_pages;
		std::vector<ServerLocation>						m_locations;						
		std::map<Ws::path, const ServerLocation*>		m_mapLocations;

		std::vector<const Ws::Sock::addr*>				m_myListenAddresses;
};

#endif
