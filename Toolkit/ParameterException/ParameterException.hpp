

#ifndef PARAMETEREXCEPTION_HPP

# define PARAMETEREXCEPTION_HPP

# include <stdexcept>
# include <cstring>
# include <cstdlib>

class ParameterException : public std::exception
{
    public:
        ParameterException(const char* caller, const char* syscall, const char* strerror);
        ParameterException(const ParameterException& copy);
        ~ParameterException() throw();
        
        const char* what() const throw();

    private:
        size_t  m_totalWritten;
        size_t  m_curLen;
        size_t  m_curWriteSize;
        char    m_buffer[1024];

        void    mf_placeInBuffer(const char* str);

        ParameterException();
        ParameterException& operator=(const ParameterException& assign);    
};

#endif