/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clock.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 15:39:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLOCK_HPP

# define CLOCK_HPP

# include "../Logs/Logs.h"
# include <ctime>
# include <sys/time.h>
# include <cerrno>
# include <cstring> 

class Clock
{
    public:
        Clock(ILog* _logFile);
        ~Clock();
        Clock(const Clock& copy);
        Clock& operator=(const Clock& assign);

        void    update();

    private:
        ILog*           _logFile;
        struct timeval  _start;
        struct timeval  _now;
        time_t          _elapsed_ms;
        char            _buffer[32];    
};


Clock::Clock(ILog* _logFile) : _logFile(_logFile), _elapsed_ms(0)
{
    if (gettimeofday(&_start, NULL) == -1)
        _logFile->record("gettimeofday(): " + std::string(std::strerror(errno)));
}

void    Clock::update()
{
    if (gettimeofday(&_now, NULL) == -1)
        _logFile->record("gettimeofday(): " + std::string(std::strerror(errno)));
    _elapsed_ms = (_now.tv_sec - _start.tv_sec) * 1000 + (_now.tv_usec - _start.tv_usec) / 1000;
}

#endif