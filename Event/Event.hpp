/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:11:05 by mmaria-d         ###   ########.fr       */
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
        Event(Globals* m_globals = NULL);
        ~Event();

        typedef void    (*HandlerFunction)(Event& event);
        typedef void*   HandlerData;


        void    setHandler_Function_and_Data(HandlerFunction function, HandlerData data);
        void    setFlags(int flags);

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
        

        
        HandlerFunction    m_function;
        HandlerData        m_data;
        t_fd               m_fd;
        int                m_flags;
        //Globals*           m_globals;

        Event(const Event& copy);
        Event& operator=(const Event& assign);
};

#endif