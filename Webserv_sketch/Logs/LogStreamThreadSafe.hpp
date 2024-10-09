/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogStreamThreadSafe.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:58:07 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:15:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGSTREAMTHREADSAFE_HPP

# define LOGSTREAMTHREADSAFE_HPP

# include "ILog.hpp"


# include <pthread.h>

# include <iostream>
# include <exception>
# include <stdexcept>
# include <cstring>
# include <cerrno>

class LogStreamThreadSafe : public ILog
{
    public:
        LogStreamThreadSafe(std::ostream& stream);
        ~LogStreamThreadSafe();

        void    record(const std::string& entry);
        void    record(const char* entry);
    private:
        std::ostream&       m_stream;
        pthread_mutex_t     m_writeLock;
};

#endif