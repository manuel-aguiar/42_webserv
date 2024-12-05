/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:56:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/04 12:02:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

//  Project Headers
# include "../../Connection/Connection.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/LogFile/LogFile.hpp"

// C++ headers
# include <queue>
# include <list>
# include <vector>

class ConnectionManager
{
	public:
		ConnectionManager(size_t maxConnections, Nginx_MemoryPool* pool, Globals* globals);
		~ConnectionManager();

		Connection*				provideConnection();
		void					returnConnection(Connection* connection);

	private:
		size_t														m_maxConnections;
		DynArray<Connection, Nginx_MemoryPool>						m_connections;
		DynArray<Event, Nginx_MemoryPool>							m_readEvents;
		DynArray<Event, Nginx_MemoryPool>							m_writeEvents;
		List<Connection*, Nginx_MPool_FixedElem<Connection*> >		m_spareConnections;

		Globals*													m_globals;


		void 					mf_destroyConnection(Connection* connection);

		ConnectionManager();
		ConnectionManager(const ConnectionManager& copy);
		ConnectionManager& operator=(const ConnectionManager& assign);
};




# endif
