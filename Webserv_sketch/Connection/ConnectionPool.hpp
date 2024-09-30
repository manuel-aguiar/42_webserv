/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:13:23 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 11:22:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CONNECTIONPOOL_HPP

# define CONNECTIONPOOL_HPP

# include "Connection.hpp"
# include "../Webserver_Definitions.h"
# include "../MemoryPool/MemoryPool.h"

# include <queue>
# include <list>

class ConnectionPool
{
    public: 
        ConnectionPool();
        ~ConnectionPool();
        
        Connection* getConnection();
        void returnConnection(Connection* connection);

    private:


        std::list<Connection*, MemoryPool_Dealloc<Connection*> >       _spareConnections;
        MemoryPool_Dealloc<Connection>                                 _pool;

        void destroyConnection(Connection* connection);

        ConnectionPool(const ConnectionPool& copy);
        ConnectionPool& operator=(const ConnectionPool& assign);
};




# endif