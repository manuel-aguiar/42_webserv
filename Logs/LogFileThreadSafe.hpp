/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFileThreadSafe.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:40:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/09 10:42:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGFILETHREADSAFE_HPP

# define LOGFILETHREADSAFE_HPP

# include "ILog.hpp"


# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>

# include <exception>
# include <stdexcept>
# include <cstring>
# include <string>
# include <cerrno>

class LogFileThreadSafe : public ILog
{

    public:
        LogFileThreadSafe(const char* filename);
        ~LogFileThreadSafe();

        void    record(const std::string& entry);
        void    record(const char* entry);


    private:
        int                 m_fd;
        pthread_mutex_t     m_writeLock;

        //Log();
        //Log(const Log& copy);
        //Log& operator=(const Log& assign);

};



#endif