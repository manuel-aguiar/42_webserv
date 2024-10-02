/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/02 08:22:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

#include <cassert>

#include "../Logs/Logs.h"

class Connection;

class Event
{
    public:
        Event(Globals* _globals = NULL);
        ~Event();

        void    setHandler(void (Event::*handler)());
        void    setConnection(Connection* connection);
        void    setFlags(int flags);

        void    accept();
        void    read();
        void    write();


        void    handle();

/*
        typedef enum
        {
            ACCEPT = 1 << 0,
            WRITE = 1 << 1,
            READ = 1 << 2
        };
*/
    //private:
        typedef void (Event::*EventHandler)();

        Connection*     _connection;
        EventHandler    _handler;
        int             _flags;
        Globals*        _globals;

        Event(const Event& copy);
        Event& operator=(const Event& assign);
};

#endif