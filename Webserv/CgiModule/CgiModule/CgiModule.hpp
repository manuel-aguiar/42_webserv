/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/15 16:02:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

// Toolkit Headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"
# include "../../../Toolkit/List/List.hpp"

// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../Cgi_Definitions.h"
# include "../CgiRequestData/CgiRequestData.hpp"

# include "../../TimerTracker/TimerTracker.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

// C++ headers
# include <map>

class CgiRequestData;
class Globals;

class CgiModule
{
	public:
		CgiModule(size_t workerCount, size_t backlogCount, size_t maxTimeout, Globals& globals);
		~CgiModule();

		// methods
		void									addInterpreter(const std::string& extension, const std::string& path);
		void									removeInterpreter(const std::string& extension);
		
		CgiRequestData*							acquireRequestData();

		void									EnqueueRequest(CgiRequestData& data);
		void									finishRequest(CgiRequestData& data);

		int										processRequests();
		
		void									StopAndReset();

		//getters
		size_t									getBusyWorkerCount() const;
		size_t									getQueueSize() const;
		const t_CgiEnvValue*					getBaseEnvKeys() const;
		const std::map<t_extension, t_path>&	getInterpreters() const;


		#define									CGI_MIN_TIMEOUT 1


	private:

		// internal classes, not exposed to the user
		class 										InternalCgiWorker;
		class										InternalCgiRequestData;

		size_t										m_numWorkers;
		size_t										m_backlog;
		size_t										m_maxTimeout;
		size_t										m_busyWorkerCount;

		// members fixed in place
		HeapArray<InternalCgiWorker>				m_allWorkers;
		HeapArray<InternalCgiRequestData>			m_allRequestData;
		
		// queues for available workers, request data and execution
		HeapCircularQueue<InternalCgiWorker*>		m_availableWorkers;
		HeapCircularQueue<InternalCgiRequestData*>	m_availableRequestData;
		HeapCircularQueue<InternalCgiRequestData*>	m_executionQueue;

		std::map<t_extension, t_path>				m_Interpreters;

		t_CgiEnvValue								m_baseEnv[E_CGI_ENV_COUNT];

		Globals&									m_globals;
		
		TimerTracker<Timer, InternalCgiRequestData*>
													m_timerTracker;


		void										mf_execute(InternalCgiWorker& worker, InternalCgiRequestData& data);

		void										mf_markWorkerForCleanup(InternalCgiWorker& worker);
		void										mf_markRequestForCleanup(InternalCgiRequestData& data);
		
		void										mf_recycleFailedStart(InternalCgiWorker& worker, InternalCgiRequestData& data, e_CgiCallback callUser);
		
		void										mf_returnWorker(InternalCgiWorker& worker);
		void										mf_returnRequestData(InternalCgiRequestData& data);
		
		void										mf_cleanupFinishedRequests();
		int											mf_finishTimedOut();
		void										mf_reloadWorkers();
		

		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &assign);

};



#endif