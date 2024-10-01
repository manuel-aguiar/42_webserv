/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:24:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 07:46:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGFILE_HPP

# define LOGFILE_HPP

# include "ILog.hpp"


# include <unistd.h>
# include <fcntl.h>


# include <exception>
# include <stdexcept>
# include <cstring>
# include <string>
# include <cerrno>

class LogFile : public ILog
{

    public:
        LogFile(const char* filename);
        ~LogFile();

        void    record(const std::string& entry);
        void    record(const char* entry);


    private:
        int         _fd;


        //Log();
        //Log(const Log& copy);
        //Log& operator=(const Log& assign);

};



#endif