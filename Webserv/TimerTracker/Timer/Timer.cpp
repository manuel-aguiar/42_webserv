

#include "Timer.hpp"

Timer::Timer() :
    m_time((struct timeval){})

{
    m_time.tv_sec = 0;
    m_time.tv_usec = 0;
}

Timer::Timer(long seconds, long microseconds)
{
    m_time.tv_sec = seconds;
    m_time.tv_usec = microseconds % 1000000;
    m_time.tv_sec += microseconds / 1000000;
}

Timer::Timer(const Timer& other)
{
    *this = other;
}

Timer& Timer::operator=(const Timer& other)
{
    if (this == &other)
        return (*this);
        
    m_time.tv_sec = other.m_time.tv_sec;
    m_time.tv_usec = other.m_time.tv_usec;
    return (*this);
}

Timer::~Timer() {}

Timer Timer::now() {
    Timer t;
    gettimeofday(&t.m_time, NULL);
    return t;
}

void    Timer::update()
{
    gettimeofday(&m_time, NULL);
}

long Timer::getSeconds() const
{
    return m_time.tv_sec;
}

long Timer::getMicroseconds() const   
{
    return m_time.tv_usec;
}

bool Timer::operator==(const Timer& other) const   
{
    return m_time.tv_sec == other.m_time.tv_sec && m_time.tv_usec == other.m_time.tv_usec;
}

bool Timer::operator!=(const Timer& other) const   
{
    return !(*this == other);
}

bool Timer::operator<(const Timer& other) const
{
    return m_time.tv_sec < other.m_time.tv_sec ||
           (m_time.tv_sec == other.m_time.tv_sec && m_time.tv_usec < other.m_time.tv_usec);
}

bool Timer::operator<=(const Timer& other) const   
{
    return *this < other || *this == other;
}

bool Timer::operator>(const Timer& other) const
{
    return !(*this <= other);
}

bool Timer::operator>=(const Timer& other) const   
{
    return !(*this < other);
}
#include <iostream>
Timer Timer::operator+(long milliseconds) const
{
    long total_microseconds = milliseconds * 1000;
    long new_seconds = m_time.tv_sec + total_microseconds / 1000000;
    long new_microseconds = m_time.tv_usec + total_microseconds % 1000000;
    
    if (new_microseconds >= 1000000)
    {
        new_seconds += 1;
        new_microseconds -= 1000000;
    }
    return Timer(new_seconds, new_microseconds);
}

Timer Timer::operator-(const Timer& other) const
{
    long sec_diff = m_time.tv_sec - other.m_time.tv_sec;
    long usec_diff = m_time.tv_usec - other.m_time.tv_usec;

    if (usec_diff < 0)
    {
        sec_diff -= 1;
        usec_diff += 1000000;
    }

    return Timer(sec_diff, usec_diff);
}

Timer& Timer::operator+=(long milliseconds)
{
    *this = *this + milliseconds;
    return (*this);
}

unsigned int Timer::getMilliseconds() const
{
    return m_time.tv_sec * 1000 + m_time.tv_usec / 1000;
}
