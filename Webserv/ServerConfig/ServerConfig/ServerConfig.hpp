

#ifndef SERVERCONFIG_HPP

# define SERVERCONFIG_HPP

// C++ Headers
# include <cstddef>
# include <set>
# include <map>
# include <vector>
# include <string>
# include <fstream>
# include <sstream>
# include <iostream>

//Own headers
# include "../../Ws_Namespace.h"

// for the KeySetters
# include "../../../Toolkit/MemoryPool/Heap_ObjectPool/Heap_ObjectPool.hpp"

// forward declarations
class Globals;
class ServerBlock;
class ServerLocation;
struct DefaultConfig;

namespace Config
{
	typedef std::string CgiExtension;
	typedef std::string CgiInterpreter;
	typedef std::map<CgiExtension, CgiInterpreter> CgiInterpreterMap;
}



class ServerConfig
{
	public:
		ServerConfig(const char* configFilePath, const DefaultConfig& defaultConfig);
		~ServerConfig();
		ServerConfig &operator=(const ServerConfig &other);

		// Getters & Setters
		const char*									getConfigPath() const;
		const std::vector<ServerBlock>&				getServerBlocks() const;
		int											getMaxConnections() const;
		int											getMaxConcurrentCgi() const;
		int											getMaxCgiBacklog() const;
		int											getMaxWorkers() const;
		size_t										getClientBodySize() const;
		size_t										getClientHeaderSize() const;
		int											getTimeoutFullHeader() const;
		int											getTimeoutInterSend() const;
		int											getTimeoutInterReceive() const;
		const std::vector<Ws::BindInfo>&			getAllBindAddresses() const;
		const DefaultConfig&						getDefaultConfig() const;
		const Config::CgiInterpreterMap&	
													getCgiInterpreters() const;

		void										setMaxConnections(const std::string &value);
		void										setMaxConcurrentCgi(const std::string &value);
		void										setMaxCgiBacklog(const std::string &value);
		void										setMaxWorkers(const std::string &value);
		void										setClientBodySize(const std::string &value);
		void										setClientHeaderSize(const std::string &value);
		void										setTimeoutFullHeader(const std::string &value);
		void										setTimeoutInterSend(const std::string &value);
		void										setTimeoutInterReceive(const std::string &value);		
		void										addCgiInterpreter(const std::string &value);

		int											parseConfigFile();
		// Debug
		void										printProgramConfig() const;
		void										printConfigs() const;

	private:
		ServerConfig(const ServerConfig &other);

		enum config_levels
		{
			PROGRAM_LEVEL,
			SERVER_LEVEL,
			LOCATION_LEVEL
		};

		struct DirectiveToSetter
		{
			DirectiveToSetter();
			typedef void 															(ServerConfig::*Setter)(const std::string &);
			typedef std::pair<const std::string, Setter> 							DirectiveSetterPair;
			typedef Heap_ObjectPool<DirectiveSetterPair>							mapPool;
			typedef std::map<std::string, Setter, std::less<std::string>, mapPool> 	DirectiveSetterMap;

			DirectiveSetterMap	map;
		};

		// common to all instances
		static DirectiveToSetter			m_directiveToSetter;

		int									m_max_connections;
		int									m_max_concurrent_cgi;
		int									m_max_cgi_backlog;
		int									m_max_workers;
		size_t								m_http_maxClientBodySize;
		size_t								m_http_maxClientHeaderSize;
		int									m_http_timeoutFullHeader;
		int									m_http_timeoutInterSend;
		int									m_http_timeoutInterReceive;
		const DefaultConfig&				m_configDefault;
		const char*							m_configFilePath;
		std::ifstream						m_configFileStream;
		size_t								m_serverCount;
		std::vector<ServerBlock>			m_serverBlocks; // m_serverBlocks is the end result of the parsing process
		std::vector<Ws::BindInfo>			m_bindAddresses;
		Config::CgiInterpreterMap	
											m_cgiInterpreters;

		// One function for parsing lines seems easier to maintain than 3 (program, server, location)
		// due to most of the parsing process being the same for all levels
		int									m_parseConfigLine(const std::string &line, const size_t &currentLine,
															std::vector<ServerBlock> &servers,
															const int &currentLevel);
		void								m_setConfigValue(const std::string &key, const std::string &value);
		bool								m_updateFile();
		void								m_setDefaults();
		bool								m_handleClosingBracket(int &currentLevel, size_t currentLine,  
															std::vector<ServerBlock> &servers);

		bool								mf_expandCgiToLocations();
		bool								mf_listenDNSlookup();
};

#endif
