/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clock.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:08:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLOCK_HPP

# define CLOCK_HPP

# include <ctime>
# include <sys/time.h>
# include <cerrno>
# include <cstring> 
# include <cassert>

class Globals;

class Clock
{
    public:
        Clock(Globals* globals = NULL);
        ~Clock();
        Clock(const Clock& copy);
        Clock& operator=(const Clock& assign);

        void                setGlobals(Globals& logFile);
        void                start();    
        void                update();
        const char*         get_FormatedTime();

    private:
        Globals*            m_globals;
        struct timeval      _start;
        struct timeval      _now;
        time_t              _elapsed_ms;
        char                _buffer[32];


        //helpers
        int                 _get_time();
};




#endif