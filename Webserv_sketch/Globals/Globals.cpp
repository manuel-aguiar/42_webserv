/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Globals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:14:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:09:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Globals.hpp"
#include "../Clock/Clock.hpp"
#include "../Logs/ILog.hpp"

Globals::Globals(Clock* clock, LogFile* logFile, LogFile* debugFile) : 
    m_clock(clock), 
    m_logFile(logFile), 
    m_debugFile(debugFile)
{
    if (m_clock && m_logFile && m_debugFile)
    {
        m_clock->setGlobals(*this);
        m_logFile->setGlobals(*this);
        m_debugFile->setGlobals(*this);
    }
}

void    Globals::setClockAndLogs(Clock& clock, LogFile& logFile, LogFile& debugFile)
{
    m_clock = &clock;
    m_logFile = &logFile;
    m_debugFile = &debugFile;

    m_clock->setGlobals(*this);
    m_logFile->setGlobals(*this);
    m_debugFile->setGlobals(*this);
}

inline
void    Globals::setGlobals()
{
    m_clock->setGlobals(*this);
    m_logFile->setGlobals(*this);
    m_debugFile->setGlobals(*this);
}

Globals::~Globals()
{

}


