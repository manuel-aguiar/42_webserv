/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:13:23 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 17:33:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CONNECTIONPOOL_HPP

# define CONNECTIONPOOL_HPP

# include "Connection.hpp"
# include "../Webserver_Definitions.h"
# include "../MemoryPool/MemoryPool.h"
# include "../Logs/Logs.h"

# include <queue>
# include <list>
# include <vector>

# define MAX_CONNECTIONS 3

class ConnectionPool
{
    public: 
        ConnectionPool(Globals* _globals, size_t maxConnections = MAX_CONNECTIONS);
        ~ConnectionPool();
        
        Connection*     getConnection();
        void            returnConnection(Connection* connection);

    private:
        
        Globals*                                                       _globals;
        size_t                                                         _maxConnections;
        
        std::vector<Connection>                                        _connections;
        std::vector<Event>                                             _readEvents;
        std::vector<Event>                                             _writeEvents;
        std::list<Connection*, MPool_FixedElem<Connection*> >          _spareConnections;

        
        void destroyConnection(Connection* connection);

        ConnectionPool();
        ConnectionPool(const ConnectionPool& copy);
        ConnectionPool& operator=(const ConnectionPool& assign);
};




# endif