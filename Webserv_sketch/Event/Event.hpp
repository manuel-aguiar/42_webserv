/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 12:31:54 by mmaria-d         ###   ########.fr       */
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

        typedef void    (*HandlerFunction)(Event& event);
        typedef void*   HandlerData;

        void    setHandler_Function_and_Data(HandlerFunction function, HandlerData data);
        void    setDataHandled(void* DataHandled);
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
        

        
        HandlerFunction    _function;
        HandlerData        _data;
        int                _flags;
        Globals*           _globals;

        Event(const Event& copy);
        Event& operator=(const Event& assign);
};

#endif