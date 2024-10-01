/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:13:23 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 08:09:48 by mmaria-d         ###   ########.fr       */
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

# define MAX_CONNECTIONS 100

class ConnectionPool
{
    public: 
        ConnectionPool(ILog* logFile, int maxConnections = MAX_CONNECTIONS);
        ~ConnectionPool();
        
        Connection*     getConnection();
        void            returnConnection(Connection* connection);

    private:
        

        std::vector<Connection>                                        _connections;
        std::vector<Event>                                             _readEvents;
        std::vector<Event>                                             _writeEvents;
        std::list<Connection*, MemoryPool_Dealloc<Connection*> >       _spareConnections;
        ILog*                                                          _logFile;

        size_t                                                         _maxConnections;
        
        void destroyConnection(Connection* connection);

        ConnectionPool();
        ConnectionPool(const ConnectionPool& copy);
        ConnectionPool& operator=(const ConnectionPool& assign);
};




# endif