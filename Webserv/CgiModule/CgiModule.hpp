/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:25:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

//Project Headers
# include "../GenericUtils/Webserver_Definitions.h"

class CgiLiveRequest;
class CgiPendingRequest;
class Globals;

class CgiModule
{
	public:
		CgiModule(size_t maxConnections, Globals& globals);
		~CgiModule();
		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &copy);

	private:
		size_t																		m_maxConnections;
		DynArray<CgiLiveRequest>													m_liveRequests;
		List<CgiPendingRequest, MemoryPool_Dealloc<CgiPendingRequest, 4096> >		m_pendingRequests;
		List<CgiLiveRequest*, MPool_FixedElem<Connection*> >						m_spareLiveRequests;


		Globals&																		m_globals;

};



#endif