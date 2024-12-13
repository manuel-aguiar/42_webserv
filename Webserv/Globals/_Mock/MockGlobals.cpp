/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockGlobals.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:14:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 09:00:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Globals.hpp"
#include "../Clock/Clock.hpp"
#include "../LogFile/LogFile.hpp"

Globals::Globals(Clock* clock, LogFile* statusFile, LogFile* errorFile, LogFile* debugFile)
{
	(void)clock;
	(void)statusFile;
	(void)errorFile;
	(void)debugFile;
}

void	Globals::setClockAndLogs(Clock& clock, LogFile& statusFile, LogFile& errorFile, LogFile& debugFile)
{
	(void)clock;
	(void)statusFile;
	(void)errorFile;
	(void)debugFile;
}

void	Globals::logStatus(const std::string& message)
{
	(void)message;
}

void	Globals::logDebug(const std::string& message)
{
	(void)message;
}

void	Globals::logError(const std::string& message)
{
	(void)message;
}

void	Globals::logStatus(const char* message)
{
	(void)message;
}

void	Globals::logDebug(const char* message)
{
	(void)message;
}

void	Globals::logError(const char* message)
{
	(void)message;
}

inline
void    Globals::mf_setGlobals()
{

}

Clock*              Globals::getClock() const {return (NULL);}
LogFile*            Globals::getStatusFile() const {return (NULL);}
LogFile*            Globals::getErrorFile() const {return (NULL);}
LogFile*            Globals::getDebugFile() const {return (NULL);}
void 			  	Globals::setClock(Clock* clock) {(void)clock;}
void 			  	Globals::setStatusFile(LogFile* statusFile) {(void)statusFile;}
void 			  	Globals::setErrorFile(LogFile* errorFile) {(void)errorFile;}
void 			  	Globals::setDebugFile(LogFile* debugFile) {(void)debugFile;}

Globals::~Globals()
{

}


