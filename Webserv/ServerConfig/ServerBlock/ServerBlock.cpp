/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:19:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/10 10:15:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

ServerBlock::ServerBlock()
{
	m_keys["listen"]				= &ServerBlock::setListener;
	m_keys["server_names"]			= &ServerBlock::addServerName;
	m_keys["client_body_size"]		= &ServerBlock::setClientBodySize;
	m_keys["client_header_size"]	= &ServerBlock::setClientHeaderSize;
	m_keys["root"]					= &ServerBlock::setRootPath;
	m_keys["error_pages"]			= &ServerBlock::addErrorPage;

	m_config["listen"];
	m_config["host"];
	m_config["port"];
	m_config["server_names"];
	m_config["client_body_size"];
	m_config["client_header_size"];
	m_config["root"];
	m_config["error_pages"];
}

ServerBlock::~ServerBlock()
{
	// Nothing to do here
}

ServerBlock &ServerBlock::operator=(const ServerBlock &other)
{
	if (this != &other)
	{
		m_config = other.m_config;
		m_keys = other.m_keys;
		m_locations = other.m_locations;
	}
	return (*this);
}

ServerBlock::ServerBlock(const ServerBlock &other)
{
	*this = other;
}

void	ServerBlock::setListener(const std::string &value, const int &flag)
{
	std::string	hostname;
	std::string	port;
	size_t		portValue;
	size_t		colonPos;

	if (!flag && !m_config["listen"].empty())
		throw (std::invalid_argument("listener already set"));

	colonPos = value.find(':');
	if (colonPos == 0 || colonPos == value.length() - 1)
		throw (std::invalid_argument("Error: Invalid format. The input should be in 'hostname:port' format.\n"));
	if (colonPos == std::string::npos)
	{
		hostname = DEFAULTCONF_BLOCK_IP_LISTEN;
		port = value;
	}
	else
	{
		if (value.find(':', colonPos + 1) != std::string::npos)
			throw (std::invalid_argument("Error: Invalid format. The input should be in 'hostname:port' format.\n"));
		hostname = value.substr(0, colonPos);
		port = value.substr(colonPos + 1);
	}
	portValue = std::stoull(port); // fix throw
	if (!isNumber(port) || portValue <= 0 || portValue > 65535)
		throw (std::invalid_argument("Error: Invalid port number. Port must be a number between 1 and 65535.\n"));
	m_config["listen"].insert(hostname + ':' + port);
}

// bool	ServerBlock::setHost(const std::string &value, const int &flag)
// {
// 	if (!flag && !m_config["host"].empty())
// 		throw (std::invalid_argument("host already set"));
// 	// if (!validateIpv4(value))
// 	// 	throw (std::invalid_argument("not an ipv4 address"));
// 	m_config["host"].clear();
// 	m_config["host"].insert(value);
// 	return (1);
// }

// bool	ServerBlock::setPort(const std::string &value, const int &flag)
// {
// 	if (!flag && !m_config["port"].empty())
// 		throw (std::invalid_argument("port already set"));
// 	if (!isNumber(value))
// 		return (0);
// 	m_config["port"].clear();
// 	m_config["port"].insert(value);

// 	return (1);
// }

void	ServerBlock::setRootPath(const std::string &value, const int &flag)
{
	if (!flag && !m_config["root"].empty())
		throw (std::invalid_argument("root path already set"));

	m_config["root"].clear();
	m_config["root"].insert(value);
}

void	ServerBlock::setClientBodySize(const std::string &value, const int &flag)
{
	if (!flag && !m_config["client_body_size"].empty())
		throw (std::invalid_argument("client body size already set"));
	try {
		parse_size(value);
	}
	catch (std::exception &e) {
		throw(e.what());
	}
	m_config["client_body_size"].clear();
	m_config["client_body_size"].insert(value);
}

void	ServerBlock::setClientHeaderSize(const std::string &value, const int &flag)
{
	if (!flag && !m_config["client_header_size"].empty())
		throw (std::invalid_argument("client header size already set"));
	try {
		parse_size(value);
	}
	catch (std::exception &e) {
		throw(e.what());
	}
	m_config["client_header_size"].clear();
	m_config["client_header_size"].insert(value);
}

void	ServerBlock::addServerName(const std::string &value, const int &flag)
{
	(void)flag;
	if (m_config["server_names"].find(value) != m_config["server_names"].end())
		throw (std::invalid_argument("server name already set"));
	m_config["server_names"].insert(value);
}

void	ServerBlock::addErrorPage(const std::string &value, const int &flag)
{
	(void)flag;
	std::stringstream	ss;
	std::string			error_code;
	std::string			path;
	size_t				separator;

	ss << value;
	separator = value.find_first_of(':', 0);
	if (separator == std::string::npos)
		throw (std::invalid_argument("no separator \":\" found while adding error page (errorValue:path)"));
	error_code = value.substr(0, separator);
	if (!isNumber(error_code))
		throw (std::invalid_argument("error code is not a number: " + error_code));
	// path = value.substr(separator + 1, value.size() - (separator - 1));			// To retrieve the path
	m_config["error_pages"].insert(ss.str());
}

void	ServerBlock::addConfigValue(const std::string &key, const std::string &value)
{
	if (m_keys.find(key) == m_keys.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_keys[key])(value, 0);
}

const std::map<std::string, ServerLocation>&		ServerBlock::getLocations() const
{
	return (m_locations);
}

// const std::string&	ServerBlock::getHost() const
// {
// 	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("host");

// 	if (it != m_config.end())
// 		return (*it->second.begin());
// 	else
// 		throw std::out_of_range("Key not found");
// }

// const std::string&	ServerBlock::getPort() const
// {
// 	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("port");
	
// 	if (it != m_config.end())
// 		return (*it->second.begin());
// 	else
// 		throw std::out_of_range("Key not found");
// }

const std::set<std::string>&	ServerBlock::getListener() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("listen");

	if (it != m_config.end())
		return (it->second);
	else
		throw std::out_of_range("Key not found");
}

const std::set<std::string>&	ServerBlock::getServerNames() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("server_names");

	if (it != m_config.end())
		return (it->second);
	else
		throw std::out_of_range("Key not found");
}

size_t	ServerBlock::getClientBodySize() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("client_body_size");

	if (it != m_config.end())
		return (parse_size(*it->second.begin()));
	else
		throw std::out_of_range("Key not found");
}

size_t	ServerBlock::getClientHeaderSize() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("client_header_size");

	if (it != m_config.end())
		return (parse_size(*it->second.begin()));
	else
		throw std::out_of_range("Key not found");
}

const std::set<std::string>&	ServerBlock::getErrorPages() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("error_pages");

	if (it != m_config.end())
		return (it->second);
	else
		throw std::out_of_range("Key not found");
}

const std::string&	ServerBlock::getRoot() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("root");

	if (it != m_config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

void	ServerBlock::setDefaults(const int &flag)
{
	DefaultConfig	defaults;

	try {
		setClientBodySize(defaults.maxClientBodySize, flag);
	}
	catch (std::exception &e) {}
	try {
		setClientHeaderSize(defaults.maxClientHeaderSize, flag);
	}
	catch (std::exception &e) {}
}

bool	ServerBlock::validate() const
{
	if (m_config.find("listen")->second.empty())
	{
		std::cerr << "Error: server config validation: listening port not assigned" << std::endl;
		return (0);
	}
	if (m_config.find("server_names")->second.empty())
	{
		std::cerr << "Error: server config validation: server_names not assigned" << std::endl;
		return (0);
	}
	if (m_config.find("root")->second.empty())
	{
		std::cerr << "Error: server config validation: root path not assigned" << std::endl;
		return (0);
	}

	return (1);
}

void	ServerBlock::setLocations(const std::vector<ServerLocation> &locations)
{
	if (locations.empty())
		return ;
	
	std::vector<ServerLocation>::const_iterator it = locations.begin();

	for (; it != locations.end(); it++)
	{
		m_locations[it->getPath()] = *it;
	}
}

// Debug

void	ServerBlock::printServerConfig() const
{
	std::set<std::string>	server_names = getServerNames();
	std::set<std::string>	error_pages = getErrorPages();
	
	std::cout << "║ ┌─ Server ─────────o" << std::endl;
	std::cout << "║ │ " <<  std::endl ;
	std::cout << "║ │ listeners: ";
	for(std::set<std::string>::const_iterator it = getListener().begin(); it != getListener().end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "║ │ Server_names: ";
	if (!server_names.size())
		std::cout << "(empty)";
	else
		for (std::set<std::string>::const_iterator it = server_names.begin(); it != server_names.end(); it++)
			std::cout << *it << " ";
	std::cout << std::endl;

	std::cout << "║ │ Client Body Size: " << getClientBodySize() << '\n';
	std::cout << "║ │ Client Header Size: " << getClientHeaderSize() << '\n';

	std::cout << "║ │ Root: " << getRoot() << std::endl;

	std::cout << "║ │ Error_pages: ";
	if (!error_pages.size())
		std::cout << "(empty)";
	else
		for (std::set<std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); it++)
			std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "║ │ " <<  std::endl ;

}


const std::vector<t_listeners>& ServerBlock::getListeners() const
{
	return (m_listeners);
}