

// Own Headers
# include "./ServerLocation.hpp"
# include "../DefaultConfig/DefaultConfig.hpp"
# include "../ServerConfig/ServerConfig.hpp"
# include "../../GenericUtils/Validation/Validation.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Ws_Namespace.h"

ServerLocation::DirectiveToSetter ServerLocation::m_directiveToSetter;

ServerLocation::DirectiveToSetter::DirectiveToSetter() :
	map(std::less<std::string>(), mapPool(6)),									// 6 magic: number of keys
	validTypes(std::less<std::string>(), Heap_ObjectPool<std::string>(3)),		// 3 magic: number of types
	validMethods(std::less<std::string>(), Heap_ObjectPool<std::string>(3))		// 3 magic: number of methods
{
	map["path"]				= &ServerLocation::setPath;
	map["root"]				= &ServerLocation::setRoot;
	map["type"]				= &ServerLocation::setType;
	map["auto_index"]		= &ServerLocation::setAutoindex;
	map["methods"]			= &ServerLocation::addMethod;
	map["cgi"]				= &ServerLocation::addCgiInterpreter;

	validTypes.insert("regular"); 
	validTypes.insert("redirection");
	validTypes.insert("cgi");

	validMethods.insert("get");
	validMethods.insert("post");
	validMethods.insert("delete");
}

ServerLocation::ServerLocation() :
	m_autoIndex(DefaultConfig::UINT_NONE) {}


ServerLocation::~ServerLocation()
{
	// Nothing to do here
}

ServerLocation &ServerLocation::operator=(const ServerLocation &other)
{
	if (this == &other)
		return (*this);

	m_path = other.m_path;
	m_root = other.m_root;
	m_type = other.m_type;
	m_autoIndex = other.m_autoIndex;
	m_methods = other.m_methods;
	m_cgiInterpreters = other.m_cgiInterpreters;

	return (*this);
}

ServerLocation::ServerLocation(const ServerLocation &other) :
	m_path				(other.m_path),
	m_root				(other.m_root),
	m_type				(other.m_type),
	m_autoIndex			(other.m_autoIndex),
	m_methods			(other.m_methods),
	m_cgiInterpreters	(other.m_cgiInterpreters) {}

void	ServerLocation::addConfigValue(const std::string &key, const std::string &value)
{
	if (m_directiveToSetter.map.find(key) == m_directiveToSetter.map.end())
		throw (std::invalid_argument("invalid key " + key));
	(this->*m_directiveToSetter.map[key])(value);
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

bool	ServerLocation::getAutoIndex() const
{
	return (m_autoIndex);
}

const std::set<std::string>&	ServerLocation::getMethods() const
{
	return (m_methods);
}

const std::string&	ServerLocation::getType() const
{
	return (m_type);
}

const Config::CgiInterpreterMap&	
ServerLocation::getCgiInterpreters() const
{
	return (m_cgiInterpreters);
}

Config::CgiInterpreterMap&
ServerLocation::accessCgiInterpreters()
{
	return (m_cgiInterpreters);
}

void		ServerLocation::setType(const std::string &value)
{
	m_directiveToSetter.validTypes.find(value);
	if (m_directiveToSetter.validTypes.find(value) == m_directiveToSetter.validTypes.end())
		throw (std::invalid_argument("invalid type value"));
	m_type = value;
}

void		ServerLocation::setPath(const std::string &value)
{
	m_path = value;
}

void		ServerLocation::setRoot(const std::string &value)
{
	m_root = value;
}

void		ServerLocation::setAutoindex(const std::string &value)
{
	if (value.size() != 1 || (value[0] != '0' && value[0] != '1'))
		throw (std::invalid_argument("invalid autoIndex value"));
	m_autoIndex = (value[0] == '1');
}

void		ServerLocation::addMethod(const std::string &value)
{
	std::string	lowercaseStr = StringUtils::strToLower(value);
	
	if (m_directiveToSetter.validMethods.find(lowercaseStr) == m_directiveToSetter.validMethods.end())
		throw (std::invalid_argument("invalid method"));
	if (m_methods.find(lowercaseStr) == m_methods.end())
		m_methods.insert(lowercaseStr);
}

void	ServerLocation::addCgiInterpreter(const std::string &value)
{
	std::string	extension;
	std::string	path("");
	size_t		colonPos;

	colonPos = value.find(':');
	if (colonPos == 0 || colonPos == value.length() - 1)
		goto exitError;
	if (value.find(':', colonPos + 1) != std::string::npos)
		goto exitError;
	extension = value.substr(0, colonPos);


	if (extension.empty())									// allow extension only, later resolved by ServerConfig
		goto exitError;

	if (colonPos != std::string::npos)
		path = value.substr(colonPos + 1);
		
	m_cgiInterpreters[extension] = path;
	
	return ;

exitError:
	throw (std::invalid_argument("Invalid 'cgi' value. The input should be in 'extension:path' format."));
}

bool ServerLocation::fillInheritedSettings(const ServerConfig& config)
{
	const Config::CgiInterpreterMap& base = config.getCgiInterpreters();
	Config::CgiInterpreterMap::iterator thisInterpreter = m_cgiInterpreters.begin();

	for (; thisInterpreter != m_cgiInterpreters.end(); thisInterpreter++)
	{
		if (!thisInterpreter->second.empty())	//location set its own interpreter for this extension (may or may not be equal to the global one)
			continue ;

		// ServerConfig looks up the path from the extension at the program level
		Config::CgiInterpreterMap::const_iterator serverLevel;
		serverLevel = base.find(thisInterpreter->first);
		if (serverLevel != base.end())
			thisInterpreter->second = serverLevel->second;
			
		else
		{
			std::cerr << "Error: location «" << getRoot() << "» 'cgi': no interpreter for extension '" << thisInterpreter->first << "'" << std::endl;
			return (false);
		}
	}
	return (true);	
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

void	ServerLocation::setDefaults(const DefaultConfig& defaultConfig)
{
	std::istringstream iss(defaultConfig.loc_http_methods);
	std::string value;

	if (m_type.empty())
		setType(defaultConfig.loc_type);
	if (m_methods.empty())
		while (iss >> value)
			addMethod(value);
	if (m_autoIndex == DefaultConfig::UINT_NONE)
		m_autoIndex = defaultConfig.loc_autoIndex;
}

void		ServerLocation::printLocationConfig() const
{
	std::cout << "║ │ ┌─ Location ──┐\n";
	std::cout << "║ │ │ path: " << getPath() << "\n";
	std::cout << "║ │ │ root: " << getRoot() << "\n";
	std::cout << "║ │ │ type: " << getType() << "\n";
	std::cout << "║ │ │ autoIndex: " << getAutoIndex() << "\n";
	std::cout << "║ │ └ methods: ";
	for (std::set<std::string>::const_iterator it = getMethods().begin(); it != getMethods().end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}
