/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogFile.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:24:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/18 09:01:28 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGFILE_HPP

# define LOGFILE_HPP

# include "../Webserver_Definitions.h"

// C headers
# include <unistd.h>
# include <fcntl.h>

// C++ Headers
# include <exception>
# include <stdexcept>
# include <cstring>
# include <string>
# include <cerrno>

class Globals;

class LogFile
{

    public:
        LogFile(const char* filename, Globals* globals = NULL);
        ~LogFile();

        void    setGlobals(Globals& globals);

        void    record(const std::string& entry);
        void    record(const char* entry);


    private:
        t_fd        m_fd;
        Globals*    m_globals;


        //Log();
        //Log(const Log& copy);
        //Log& operator=(const Log& assign);

};



#endif
