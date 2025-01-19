
#include "ParameterException.hpp"

ParameterException::ParameterException(const char* caller, const char* syscall, const char* strerror) :
    m_totalWritten(0),
    m_curLen(0),
    m_curWriteSize(0),
    m_buffer()
{
    mf_placeInBuffer(caller);
    mf_placeInBuffer(": ");
    mf_placeInBuffer(syscall);
    mf_placeInBuffer("(): ");
    mf_placeInBuffer(strerror);
    m_buffer[m_totalWritten] = '\0';
}

ParameterException::ParameterException(const ParameterException& copy) :
    m_totalWritten(copy.m_totalWritten),
    m_curLen(0),
    m_curWriteSize(0)
{
    std::memcpy(m_buffer, copy.m_buffer, sizeof(m_buffer));
}

ParameterException::~ParameterException() throw() {}

const char* ParameterException::what() const throw()
{
    return (m_buffer);
}

void    ParameterException::mf_placeInBuffer(const char* str)
{
    m_curLen = strlen(str);
    m_curWriteSize = std::min(sizeof(m_buffer) - m_totalWritten - 1, m_curLen);
    std::memcpy(&m_buffer[m_totalWritten], str, m_curWriteSize);
    m_totalWritten += m_curWriteSize;
}



ParameterException::ParameterException() {}

ParameterException& ParameterException::operator=(const ParameterException& assign) {(void)assign; return (*this);}  