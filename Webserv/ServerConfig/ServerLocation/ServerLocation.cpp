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
	if (this == &other)
		return (*this);
	m_keys = other.m_keys;
	m_path = other.m_path;
	m_root = other.m_root;
	m_type = other.m_type;
	m_autoIndex = other.m_autoIndex;
	m_methods = other.m_methods;
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

const std::string&	ServerLocation::getPath() const
{
	return (m_path);
}

const std::string&	ServerLocation::getRoot() const
{
	return (m_root);
}

bool	ServerLocation::getAutoindex() const
{
	return (m_autoIndex);
}

const std::set<std::string>&	ServerLocation::getMethods() const
{
	return (m_methods);
}

std::string	ServerLocation::getType() const
{
	return (m_type);
}

void		ServerLocation::setType(const std::string &value, const int &flag)
{
	if (!flag && !m_type.empty())
		throw (std::invalid_argument("type already set"));
	if (m_validTypes.find(value) == m_validTypes.end())
		throw (std::invalid_argument("invalid type value"));
	m_type = value;
}

void		ServerLocation::setPath(const std::string &value, const int &flag)
{
	if (!flag && !m_path.empty())
		throw (std::invalid_argument("path path already set"));
	m_path = value;
}

void		ServerLocation::setRoot(const std::string &value, const int &flag)
{
	if (!flag && !m_root.empty())
		throw (std::invalid_argument("root path already set"));
	m_root = value;
}

void		ServerLocation::setAutoindex(const std::string &value, const int &flag)
{
	(void)flag;
	// if (!flag && !m_autoIndex)
	// 	throw (std::invalid_argument("autoIndex already set"));
	if (value.size() != 1 || (value[0] != '0' && value[0] != '1'))
		throw (std::invalid_argument("invalid autoIndex value"));
	m_autoIndex = (value[0] == '1');
}

void		ServerLocation::addMethod(const std::string &value, const int &flag)
{
	(void)flag;
	std::string	lowercaseStr = strToLower(value);
	
	if (m_validMethods.find(lowercaseStr) == m_validMethods.end())
		throw (std::invalid_argument("invalid method"));
	if (m_methods.find(lowercaseStr) != m_methods.end())
	{
		if (!flag)
			throw (std::invalid_argument("method already set"));
	}
	else
		m_methods.insert(lowercaseStr);
}


bool		ServerLocation::validate() const
{
	if (m_path.empty())
	{
		std::cerr << "Error: server config validation: path not assigned" << std::endl;
		return (0);
	}
	if (m_root.empty())
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
	std::cout << "║ │ ┌─ Location ──┐\n";
	std::cout << "║ │ │ path: " << getPath() << "\n";
	std::cout << "║ │ │ root: " << getRoot() << "\n";
	std::cout << "║ │ │ type: " << getType() << "\n";
	std::cout << "║ │ │ autoIndex: " << getAutoindex() << "\n";
	std::cout << "║ │ └ methods: ";
	for (std::set<std::string>::const_iterator it = getMethods().begin(); it != getMethods().end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}
