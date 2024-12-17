/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocations.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:56:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/18 10:57:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ServerLocation.hpp"


ServerLocation::ServerLocation()
{
	m_keys["path"]				= &ServerLocation::setPath;
	m_keys["root"]				= &ServerLocation::setRoot;
	m_keys["type"]				= &ServerLocation::setType;
	m_keys["autoIndex"]			= &ServerLocation::setAutoindex;
	m_keys["methods"]			= &ServerLocation::addMethod;

	m_config["path"];
	m_config["root"];
	m_config["type"];
	m_config["autoIndex"];
	m_config["methods"];

	m_validTypes.insert("regular"); 
	m_validTypes.insert("redirection");
	m_validTypes.insert("cgi");

	m_validMethods.insert("get");
	m_validMethods.insert("post");
	m_validMethods.insert("delete");
}

ServerLocation::~ServerLocation()
{
	// Nothing to do here
}

ServerLocation &ServerLocation::operator=(const ServerLocation &other)
{
	if (this != &other)
	{
		m_keys = other.m_keys;
		m_config = other.m_config;
	}
	return (*this);
}

ServerLocation::ServerLocation(const ServerLocation &other)
{
	*this = other;
}

void	ServerLocation::addConfigValue(const std::string &key, const std::string &value)
{
	if (m_keys.find(key) == m_keys.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_keys[key])(value, 0);
}

// Getters & Setters

const std::string&				ServerLocation::getPath() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("path");

	if (it != m_config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");

}

const std::string&				ServerLocation::getRoot() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("root");

	if (it != m_config.end())
		return (*it->second.begin());
	else
		throw std::out_of_range("Key not found");
}

bool							ServerLocation::getAutoindex() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("autoIndex");

	if (it != m_config.end())
		return (*it->second.begin() == "1");
	else
		throw std::out_of_range("Key not found");	

}

const std::set<std::string>&	ServerLocation::getMethods() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("methods");

	if (it != m_config.end())
		return (it->second);
	else
		throw std::out_of_range("Key not found");
}

int	ServerLocation::getType() const
{
	std::map<std::string, std::set<std::string> >::const_iterator it = m_config.find("type");

	if (it != m_config.end())
	{
		if (*it->second.begin() == "regular")
			return (E_LOCATION_REGULAR);
		else if (*it->second.begin() == "redirection")
			return (E_LOCATION_REDIRECTION);
		else if (*it->second.begin() == "cgi")
			return (E_LOCATION_CGI);
	}
	else
		throw std::out_of_range("Key not found");
	return (-1);
}

void		ServerLocation::setType(const std::string &value, const int &flag)
{
	if (!flag && !m_config["type"].empty())
		throw (std::invalid_argument("type already set"));
	if (m_validTypes.find(value) == m_validTypes.end())
		throw (std::invalid_argument("invalid type value"));
	m_config["type"].clear();
	m_config["type"].insert(value);

}

void		ServerLocation::setPath(const std::string &value, const int &flag)
{
	if (!flag && !m_config["path"].empty())
		throw (std::invalid_argument("path path already set"));
	m_config["path"].clear();
	m_config["path"].insert(value);
}

void		ServerLocation::setRoot(const std::string &value, const int &flag)
{
	if (!flag && !m_config["root"].empty())
		throw (std::invalid_argument("root path already set"));
	m_config["root"].clear();
	m_config["root"].insert(value);
}

void		ServerLocation::setAutoindex(const std::string &value, const int &flag)
{
	if (!flag && !m_config["autoIndex"].empty())
		throw (std::invalid_argument("autoIndex already set"));
	if ((!isNumber(value) || value.size() != 1) || (value[0] != '0' && value[0] != '1'))
		throw (std::invalid_argument("invalid autoIndex value"));
	m_config["autoIndex"].clear();
	m_config["autoIndex"].insert(value);
}

std::string	strToLower(const std::string& str)
{
	std::string lowercaseString = str;
	for (std::string::iterator it = lowercaseString.begin(); it != lowercaseString.end(); ++it)
		*it = static_cast<char>(std::tolower(static_cast<unsigned char>(*it)));
	
	return (lowercaseString);
}

void		ServerLocation::addMethod(const std::string &value, const int &flag)
{
	(void)flag;
	std::string	lowercaseStr = strToLower(value);
	
	if (m_validMethods.find(lowercaseStr) == m_validMethods.end())
		throw (std::invalid_argument("invalid method"));
	if (m_config.find(lowercaseStr) != m_config.end())
	{
		if (!flag)
			throw (std::invalid_argument("method already set"));
	}
	else
		m_config["methods"].insert(lowercaseStr);
}


bool		ServerLocation::validate() const
{
	if (m_config.find("path")->second.empty())
	{
		std::cerr << "Error: server config validation: path not assigned" << std::endl;
		return (0);
	}
	if (m_config.find("root")->second.empty())
	{
		std::cerr << "Error: server config validation: root not assigned" << std::endl;
		return (0);
	}
	return (1);
}

void	ServerLocation::setDefaults(const int &flag)
{
	DefaultConfig	defaults;
	std::istringstream iss(defaults.methods);
	std::string value;

	try {
		setType(defaults.type, flag);
	}
	catch (std::exception &e) {}
	try {

		if (getMethods().empty())
			while (iss >> value)
				addMethod(value, 1);
	}
	catch (std::exception &e) {}
	try {
		setAutoindex(defaults.autoIndex, flag);
	}
	catch (std::exception &e) {}
}

void		ServerLocation::printLocationConfig() const
{
	std::cout << "║ │ ┌─ Location ──┐" << std::endl;
	std::cout << "║ │ │ path: " << getPath() << std::endl;
	std::cout << "║ │ │ root: " << getRoot() << std::endl;
	std::cout << "║ │ │ type: " << getType() << std::endl;
	std::cout << "║ │ │ autoIndex: " << getAutoindex() << std::endl;
	std::cout << "║ │ └ methods: ";
	for (std::set<std::string>::const_iterator it = getMethods().begin(); it != getMethods().end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}
