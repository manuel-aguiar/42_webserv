/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 15:21:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

//Project Headers
# include "../GenericUtils/Webserver_Definitions.h"
# include "Cgi_Definitions.h"

# include "CgiLiveRequest/CgiLiveRequest.hpp"
# include "CgiRequestData/CgiRequestData.hpp"

// C++ headers
# include <map>

class CgiLiveRequest;
class CgiRequestData;
class Globals;

class CgiModule
{
	public:
		CgiModule(size_t maxConnections, Globals& globals);
		~CgiModule();
		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &copy);

		// methods
		void									addInterpreter(const std::string& extension, const std::string& path);
		void									removeInterpreter(const std::string& extension);
		
		CgiRequestData&							acquireRequestData();
		void									executeRequest(CgiRequestData& data);

		//getters
		const std::map<e_CgiEnv, t_CgiEnvKey>&	getBaseEnvKeys() const;
		const std::map<t_extension, t_path>&	getInterpreters() const;

	private:
		size_t																		m_maxConnections;
		DynArray<CgiLiveRequest>													m_liveRequests;
		List<CgiRequestData>														m_allRequestData;
		
		
		// no memory pool for now
		List<CgiRequestData*>														m_pendingRequests;
		List<CgiLiveRequest*>														m_spareLiveRequests;

		std::map<t_extension, t_path>												m_Interpreters;
		std::map<e_CgiEnv, t_CgiEnvKey>												m_baseEnvLeftEqual;

		Globals&																	m_globals;

};



#endif