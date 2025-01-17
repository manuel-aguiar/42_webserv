

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

// Own Headers
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../ServerLocation/ServerLocation.hpp"
# include "../DefaultConfig/DefaultConfig.hpp"
# include "../ServerBlock/ServerBlock.hpp"

// forward declarations
class Globals;

class ServerConfig
{
	public:
		ServerConfig(const char* configFilePath, Globals* globals);
		~ServerConfig();
		ServerConfig &operator=(const ServerConfig &other);

		// Getters & Setters
		const t_path&								getConfigPath() const;
		const std::map<std::string, ServerBlock>&	getServerBlocks() const;
		const std::string&							getMaxConnections() const;
		const std::string&							getMaxConcurrentCgi() const;
		const std::string&							getMaxCgiBacklog() const;
		void										setConfigPath(const t_path &path);
		void										setMaxConnections(const std::string &value);
		void										setMaxConcurrentCgi(const std::string &value);
		void										setMaxCgiBacklog(const std::string &value);

		int											parseConfigFile();
		// Debug
		void										printProgramConfig() const;
		void										printConfigs() const;

	private:
		ServerConfig();
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


		std::string							m_max_connections;
		std::string							m_max_concurrent_cgi;
		std::string							m_max_cgi_backlog;
		DefaultConfig						m_configDefault;
		t_path								m_configFilePath;
		std::ifstream						m_configFileStream;
		size_t								m_serverCount;
		std::map<std::string, ServerBlock>	m_serverBlocks; // m_serverBlocks is the end result of the parsing process
		Globals*							m_globals; // mostly for logs and debuging, see Globals class
		

		// One function for parsing lines seems easier to maintain than 3 (program, server, location)
		// due to most of the parsing process being the same for all levels
		int									m_parseConfigLine(const std::string &line, const size_t &currentLine,
															std::vector<ServerBlock> &servers,
															std::vector<ServerLocation> &locations,
															const int &currentLevel);
		void								m_setConfigValue(const std::string &key, const std::string &value);
		bool								m_updateFile();
		void								m_setServers(std::vector<ServerBlock> &servers);
		void								m_setDefaults();
		bool								m_handleClosingBracket(int &currentLevel, size_t currentLine,  
												std::vector<ServerBlock> &servers, 
												std::vector<ServerLocation> &locations);
};

#endif
