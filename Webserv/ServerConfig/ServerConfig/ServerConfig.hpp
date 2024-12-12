/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:50:49 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/12 14:47:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
class DefaultConfig;
class ServerBlock;
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
		void										setConfigPath(const t_path &path);
		void										setMaxConnections(const std::string &value, const int &flag = 0);
		void										setMaxConcurrentCgi(const std::string &value, const int &flag = 0);

		int											parseConfigFile();

		// Debug
		void										printProgramConfig() const;
		void										printConfigs() const;


		//
		size_t										getNumWorkers() const;

	private:
		enum config_levels
		{
			PROGRAM_LEVEL,
			SERVER_LEVEL,
			LOCATION_LEVEL
		};

		// Key/value storing for config settings
		typedef void (ServerConfig::*f_addConfigValue)(const std::string &, const int &);
		std::map<std::string, std::set<std::string> >		m_config;
		std::map<std::string, f_addConfigValue>				m_keys;

		ServerConfig();
		ServerConfig(const ServerConfig &other);

		DefaultConfig						m_configDefault;
		t_path								m_configFilePath;
		std::ifstream						m_configFileStream;
		size_t								m_serverCount;
		std::map<std::string, ServerBlock>	m_serverBlocks;		// std::string would be like IP:PORT:server_name as the identifier...?
																// m_serverBlocks is the end result of the parsing process
		// mostly for logs and debuging, see Globals class
		Globals*							m_globals;

		// One function for parsing lines seems easier to maintain than 3 (program, server, location)
		// due to most of the parsing process being the same for all levels
		int									m_parseConfigLine(const std::string &line, const size_t &currentLine,
															ServerBlock &server, ServerLocation &location,
															const int &currentLevel);
		void								m_setConfigValue(const std::string &key, const std::string &value);
		void								m_updateFile();
		void								m_setServers(std::vector<ServerBlock> &servers);
		void								m_setDefaults(const int &flag);
};

#endif
