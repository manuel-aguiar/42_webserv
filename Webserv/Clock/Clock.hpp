/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clock.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/16 15:44:47 by manuel           ###   ########.fr       */
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
        struct timeval      m_start;
        struct timeval      m_now;
        time_t              m_elapsed_ms;
        char                m_buffer[32];


        //helpers
        int                 _get_time();
};




#endif
