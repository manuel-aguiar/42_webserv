

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

// forward declarations
class Globals;
class ServerBlock;
class ServerLocation;
struct DefaultConfig;

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
		const std::vector<Ws::BindInfo>&			getAllBindAddresses() const;
		const DefaultConfig&						getDefaultConfig() const;
		void										setMaxConnections(const std::string &value);
		void										setMaxConcurrentCgi(const std::string &value);
		void										setMaxCgiBacklog(const std::string &value);
		void										setMaxWorkers(const std::string &value);		

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

		// Key/value storing for config settings
		typedef void (ServerConfig::*f_addConfigValue)(const std::string &);
		std::map<std::string, f_addConfigValue>				m_keys;


		int									m_max_connections;
		int									m_max_concurrent_cgi;
		int									m_max_cgi_backlog;
		int									m_max_workers;
		const DefaultConfig&				m_configDefault;
		const char*							m_configFilePath;
		std::ifstream						m_configFileStream;
		size_t								m_serverCount;
		std::vector<ServerBlock>			m_serverBlocks; // m_serverBlocks is the end result of the parsing process
		std::vector<Ws::BindInfo>			m_bindAddresses;

		// One function for parsing lines seems easier to maintain than 3 (program, server, location)
		// due to most of the parsing process being the same for all levels
		int									m_parseConfigLine(const std::string &line, const size_t &currentLine,
															std::vector<ServerBlock> &servers,
															std::vector<ServerLocation> &locations,
															const int &currentLevel);
		void								m_setConfigValue(const std::string &key, const std::string &value);
		bool								m_updateFile();
		void								m_setDefaults();
		bool								m_handleClosingBracket(int &currentLevel, size_t currentLine,  
															std::vector<ServerBlock> &servers, 
															std::vector<ServerLocation> &locations);


		bool								mf_listenDNSlookup();
};

#endif
