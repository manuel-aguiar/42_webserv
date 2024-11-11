/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/08 10:38:16 by manuel           ###   ########.fr       */
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

        HandlerFunction    m_function;
        HandlerData        m_data;
        t_fd               m_fd;
        int                m_flags;
        //Globals*           m_globals;

        Event(const Event& copy);
        Event& operator=(const Event& assign);
};

#endif
