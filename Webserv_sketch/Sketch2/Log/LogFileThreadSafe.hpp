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

class LogFileThreadSafe : public ILog
{

    public:
        LogFileThreadSafe();
        ~LogFileThreadSafe();

        void    record(const std::string& entry);
    


    private:
        int                 _fd;
        pthread_mutex_t     _writeLock;

        //Log();
        //Log(const Log& copy);
        //Log& operator=(const Log& assign);

};



#endif