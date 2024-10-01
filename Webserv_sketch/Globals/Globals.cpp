/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Globals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:14:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 18:44:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Globals.hpp"
#include "../Clock/Clock.hpp"
#include "../Logs/ILog.hpp"

Globals::Globals(Clock* clock, LogFile* logFile, LogFile* debugFile) : 
    _clock(clock), 
    _logFile(logFile), 
    _debugFile(debugFile)
{
    if (_clock && _logFile && _debugFile)
    {
        _clock->setGlobals(*this);
        _logFile->setGlobals(*this);
        _debugFile->setGlobals(*this);
    }
}

void    Globals::setClockAndLogs(Clock& clock, LogFile& logFile, LogFile& debugFile)
{
    _clock = &clock;
    _logFile = &logFile;
    _debugFile = &debugFile;

    _clock->setGlobals(*this);
    _logFile->setGlobals(*this);
    _debugFile->setGlobals(*this);
}

inline
void    Globals::setGlobals()
{
    _clock->setGlobals(*this);
    _logFile->setGlobals(*this);
    _debugFile->setGlobals(*this);
}

Globals::~Globals()
{

}


