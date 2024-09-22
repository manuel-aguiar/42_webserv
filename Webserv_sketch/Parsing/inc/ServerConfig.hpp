#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "utils.hpp"

#define KEY_AMOUNT 6

class ServerConfig
{
	private:
		typedef bool (ServerConfig::*f_addConfigValue)(const std::string &);

		std::map<std::string, std::set<std::string> > _config;
		std::map<std::string, f_addConfigValue> _keys;

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

		bool					addConfigValue(const std::string &key, const std::string &value);
};

#endif