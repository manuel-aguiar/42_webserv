

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

        void            update();

        long            getSeconds() const;
        long            getMicroseconds() const;
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
