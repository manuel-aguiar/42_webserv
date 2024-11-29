/* ************************************************************************** */
/*										      */
/*								:::      ::::::::   */
/*   ConnectionManager.hpp				     :+:      :+:    :+:   */
/*							   +:+ +:+	  +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+	+#+	 */
/*						      +#+#+#+#+#+   +#+	    */
/*   Created: 2024/09/27 16:13:23 by mmaria-d	   #+#    #+#	      */
/*   Updated: 2024/10/14 14:14:48 by mmaria-d	  ###   ########.fr	*/
/*										      */
/* ************************************************************************** */

# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

# include "../../Connection/Connection.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/LogFile/LogFile.hpp"

# include <queue>
# include <list>
# include <vector>

# define MAX_CONNECTIONS 3

class ConnectionManager
{
	public:
		ConnectionManager(Globals* m_globals, size_t maxConnections);
		~ConnectionManager();

		Connection*				provideConnection();
		void					returnConnection(Connection* connection);

	private:
		Globals*												m_globals;
		size_t													m_maxConnections;

		HeapArray<Connection>									m_connections;
		HeapArray<Event>										m_readEvents;
		HeapArray<Event>										m_writeEvents;
		List<Connection*, MPool_FixedElem<Connection*> >		m_spareConnections;

		void 					destroyConnection(Connection* connection);

		ConnectionManager();
		ConnectionManager(const ConnectionManager& copy);
		ConnectionManager& operator=(const ConnectionManager& assign);
};




# endif
