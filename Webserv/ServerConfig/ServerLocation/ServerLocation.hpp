

#ifndef SERVERLOCATION_HPP

# define SERVERLOCATION_HPP

// Project headers

// for the KeySetters
# include "../../../Toolkit/MemoryPool/Heap_ObjectPool/Heap_ObjectPool.hpp"

// C++ Headers
# include <string>
# include <set>
# include <map>
# include <iostream>
# include <exception>
# include <sstream>

class ServerConfig;
class ServerBlock;
struct DefaultConfig;
namespace Config
{
	typedef std::string CgiExtension;
	typedef std::string CgiInterpreter;
	typedef std::map<CgiExtension, CgiInterpreter> CgiInterpreterMap;
}


class ServerLocation
{
	public:

		typedef enum e_locationType
		{
			E_LOCATION_REGULAR,
			E_LOCATION_REDIRECTION,
			E_LOCATION_CGI
		}	t_locationType;
	
		ServerLocation();
		~ServerLocation();
		ServerLocation &operator=(const ServerLocation &other);
		ServerLocation(const ServerLocation &other);

		// Getters & Setters
		const std::string&				getPath() const;
		const std::string&				getRoot() const;
		bool							getAutoIndex() const;
		const std::set<std::string>&	getMethods() const;
		const std::string&				getType() const;
		const Config::CgiInterpreterMap&
										getCgiInterpreters() const;


		void							setType(const std::string &value);
		void							setPath(const std::string &value);
		void							setRoot(const std::string &value);
		void							setAutoindex(const std::string &value);
		void							addMethod(const std::string &value);
		void							addCgiInterpreter(const std::string &value);

		bool							fillInheritedSettings(const ServerConfig& config);

		void							setDefaults(const DefaultConfig& defaultConfig);
		void							addConfigValue(const std::string &key, const std::string &value);
		bool							validate() const;

		// DEBUG
		void							printLocationConfig() const;

		Config::CgiInterpreterMap&
										accessCgiInterpreters();

	private:
		
		struct DirectiveToSetter
		{
			DirectiveToSetter();
			typedef void 															(ServerLocation::*Setter)(const std::string &);
			typedef std::pair<const std::string, Setter> 							DirectiveSetterPair;
			typedef Heap_ObjectPool<DirectiveSetterPair>							mapPool;
			typedef std::map<std::string, Setter, std::less<std::string>, mapPool> 	DirectiveSetterMap;

			typedef std::set<std::string, std::less<std::string>, Heap_ObjectPool<std::string> >
																					MatchSet;
			DirectiveSetterMap	map;
			MatchSet			validTypes;
			MatchSet			validMethods;
		};

		// common to all instances
		static DirectiveToSetter		m_directiveToSetter;

		std::string						m_path;
		std::string						m_root;
		std::string						m_type;
		int								m_autoIndex;
		std::set<std::string>			m_methods;
		Config::CgiInterpreterMap		m_cgiInterpreters;

		// some cgi stuff with path and extension here
		// some redirection stuff with URL to follow here
};



#endif
