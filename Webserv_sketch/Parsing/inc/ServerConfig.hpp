#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "utils.hpp"

#define KEY_AMOUNT 6

class ServerConfig
{
	private:
		std::map<std::string, std::set<std::string> > _config;
		std::string _keys[KEY_AMOUNT] = {
			"host",
			"port",
			"server_names",
			"client_body_size",
			"root",
			"error_pages"
		};
		
		// std::set<std::string> _default_values = {
		// 	DEFAULT_HOST,
		// 	"port",
		// 	"server_names",
		// 	DEFAULT_CLIENT_BODY_SIZE,
		// 	"root",
		// 	"error_pages"
		// 	};

		bool (ServerConfig::*f_addConfigValue[KEY_AMOUNT])(const std::string &) = { 
			&ServerConfig::setHost,
			&ServerConfig::addPort,
			&ServerConfig::addServerName,
			&ServerConfig::setClientBodySize,
			&ServerConfig::setRootPath,
			&ServerConfig::addErrorPage
		};

		// std::map<std::string, LocationConfig>	locations;

	public:
		ServerConfig();

		bool					setHost(const std::string &value);
		bool					setRootPath(const std::string &value);
		bool					setClientBodySize(const std::string &value);
		bool					addPort(const std::string &value);
		bool					addServerName(const std::string &value);
		bool					addErrorPage(const std::string &value);
        std::string				getHost() const;
		std::set<std::string>	getPorts() const;
		std::set<std::string>	getServerNames() const;
		std::string				getClientBodySize() const;
		std::set<std::string>	getErrorPages() const;
		std::string				getRoot() const;

		bool					addConfigValue(std::string &key, std::string &value);
};

#endif