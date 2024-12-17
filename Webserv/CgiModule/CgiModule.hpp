/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 08:33:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

//Project Headers
# include "../GenericUtils/Webserver_Definitions.h"

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
		void				addInterpreter(const std::string& extension, const std::string& path);
		void				addRequestData(const CgiRequestData& request);

		CgiRequestData&		requireRequestData();


	private:
		size_t																		m_maxConnections;
		DynArray<CgiLiveRequest>													m_liveRequests;

		// no memory pool for now
		List<CgiRequestData>														m_pendingRequests;
		List<CgiLiveRequest*>														m_spareLiveRequests;

		typedef std::string															t_extension;
		typedef std::string															t_path;
		std::map<t_extension, t_path>												m_scriptInterpreters;		

		Globals&																	m_globals;

};



#endif