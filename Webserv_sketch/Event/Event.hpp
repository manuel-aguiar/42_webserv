/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 12:12:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

class Connection;

class Event
{
    public:
        Event();
        ~Event();

        void    setHandler(void (Event::*handler)());
        void    setConnection(Connection* connection);
        void    setFlags(int flags);

        void    accept();
        void    read();
        void    write();


    //private:
        typedef void (Event::*EventHandler)();

        Connection*     _connection;
        EventHandler    _handler;
        int             _flags;

        Event(const Event& copy);
        Event& operator=(const Event& assign);
};

#endif