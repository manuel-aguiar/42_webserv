/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:13:23 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/14 14:14:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CONNECTIONPOOL_HPP

# define CONNECTIONPOOL_HPP

# include "Connection.hpp"
# include "../Webserver_Definitions.h"
# include "../Logs/Logs.h"

# include <queue>
# include <list>
# include <vector>

# define MAX_CONNECTIONS 3

class ConnectionPool
{
	public:
		ConnectionPool(Globals* m_globals, size_t maxConnections = MAX_CONNECTIONS);
		~ConnectionPool();

		Connection*     getConnection();
		void            returnConnection(Connection* connection);

	private:
		Globals*                                                       m_globals;
		size_t                                                         m_maxConnections;

		HeapArray<Connection>                                          m_connections;
		HeapArray<Event>                                               m_readEvents;
		HeapArray<Event>                                               m_writeEvents;
		List<Connection*, MPool_FixedElem<Connection*> >               m_spareConnections;

		void destroyConnection(Connection* connection);

		ConnectionPool();
		ConnectionPool(const ConnectionPool& copy);
		ConnectionPool& operator=(const ConnectionPool& assign);
};




# endif
