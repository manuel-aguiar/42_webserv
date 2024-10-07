/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Globals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:10:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/02 08:09:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_HPP

# define GLOBALS_HPP


# include "../Webserver_Definitions.h"
# include "../Logs/Logs.h"
# include "../Clock/Clock.hpp"

class Globals
{
    public:
        Globals(Clock* clock = NULL, LogFile* logFile = NULL, LogFile* debugFile = NULL);
        ~Globals();

        void            setClockAndLogs(Clock& clock, LogFile& logFile, LogFile& debugFile);

        Clock*              _clock;
        LogFile*            _logFile;
        LogFile*            _debugFile;

    private:

        void            setGlobals();

        Globals(const Globals& copy);
        Globals& operator=(const Globals& assign);
};


#endif