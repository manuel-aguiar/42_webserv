/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:19:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/20 14:32:31 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP

# define SERVERBLOCK_HPP

// C++ headers
# include <iostream>
# include <string>
# include <map>
# include <set>
# include <vector>
# include <exception>

// Own headers

# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../GenericUtils/Validation/Validation.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../ServerLocation/ServerLocation.hpp"
# include "../DefaultConfig/DefaultConfig.hpp"
class ServerLocation;

class ServerBlock
{
	public:
		ServerBlock();
		~ServerBlock();
		ServerBlock &operator=(const ServerBlock &other);
		ServerBlock(const ServerBlock &other);

		const std::set<std::string>&						getDomainNames() const;
		const std::map<std::string, ServerLocation>& 		getLocations() const;

		void							setLocations(const std::vector<ServerLocation> &Locations);
		void							setListener(const std::string &value, const int &flag = 0);
		void							setRootPath(const std::string &value, const int &flag = 0);
		void							setClientBodySize(const std::string &value, const int &flag = 0);
		void							setClientHeaderSize(const std::string &value, const int &flag = 0);
		void							addServerName(const std::string &value, const int &flag = 0);
		void							addErrorPage(const std::string &value, const int &flag = 0);
		const std::string&				getHost() const;
		const std::string&				getPort() const;
		const std::set<std::string>&	getListener() const;
		const std::set<std::string>&	getServerNames() const;
		size_t							getClientBodySize() const;
		size_t							getClientHeaderSize() const;
		const std::set<std::string>&	getErrorPages() const;
		const std::string&				getRoot() const;

		void							setDefaults(const int &flag = 0);
		void							addConfigValue(const std::string &key, const std::string &value);
		bool							validate() const;

		// Debug
		void							printServerConfig() const;

	private:
		typedef void (ServerBlock::*f_addConfigValue)(const std::string &, const int &);
		std::map<std::string, std::set<std::string> >	m_config;
		std::map<std::string, f_addConfigValue> 		m_keys;

		std::set<std::string>							m_ServerName;		// would be easier to only accept one name per serverblock
		std::map<t_path, ServerLocation>				m_locations;

		bool							m_setHost(const std::string &value, const int &flag = 0); // Host and port are set through setListener()
		bool							m_setPort(const std::string &value, const int &flag = 0);
};



#endif
