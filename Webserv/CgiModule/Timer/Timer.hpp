/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 10:55:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 11:46:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_HPP

# define TIMER_HPP

// C++headers
#include <cstddef>

// C headers
#include <sys/time.h>

class Timer
{
    public:
        Timer();
        Timer(long seconds, long microseconds);
        Timer(const Timer& other);
        Timer& operator=(const Timer& other);
        ~Timer();

        static Timer now();

        // Getters
        long            seconds() const;
        long            microseconds() const;
        unsigned int    getMilliseconds() const;
        
        bool operator==(const Timer& other) const;
        bool operator!=(const Timer& other) const;
        bool operator<(const Timer& other) const;
        bool operator<=(const Timer& other) const;
        bool operator>(const Timer& other) const;
        bool operator>=(const Timer& other) const;

        Timer operator+(long milliseconds) const;
        Timer& operator+=(long milliseconds);
        
        Timer operator-(const Timer& other) const;

    private:
        struct timeval m_time;
};


#endif