/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/21 11:03:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

#include <cassert>

#include "../../../Globals/LogFile/LogFile.hpp"

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
