/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Globals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:10:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 11:00:24 by mmaria-d         ###   ########.fr       */
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

		void            	setClockAndLogs(Clock& clock, LogFile& logFile, LogFile& debugFile);

		void                logStatus(const std::string& message);
		void                logDebug(const std::string& message);
		void                logError(const std::string& message);

		void                logStatus(const char* message);
		void                logDebug(const char* message);
		void                logError(const char* message);

		Clock*              m_clock;
		LogFile*            m_logFile;
		LogFile*			m_errorFile;
		LogFile*            m_debugFile;

	private:

		void            setGlobals();

		Globals(const Globals& copy);
		Globals& operator=(const Globals& assign);
};


#endif