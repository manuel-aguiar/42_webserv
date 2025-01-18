/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModule.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 08:51:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 20:03:22 by mmaria-d         ###   ########.fr       */
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
# include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
// C++ headers
# include <map>

class CgiRequestData;
class Globals;

class CgiModule
{
	public:
		CgiModule(size_t workerCount, size_t backlogCount, size_t maxTimeout, EventManager& eventManager, Globals& globals);
		~CgiModule();

		// request interaction
		CgiRequestData*							acquireRequestData();
		void									enqueueRequest(CgiRequestData& data, bool isCalledFromEventLoop);
		void									finishRequest(CgiRequestData& data, bool isCalledFromEventLoop);

		// processing
		int										processRequests();
		void									stopAndReset();

		// config
		void									addInterpreter(const std::string& extension, const std::string& path);
		void									removeInterpreter(const std::string& extension);

		// getters
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

		EventManager&								m_eventManager;
		Globals&									m_globals;
		
		TimerTracker<Timer, InternalCgiRequestData*>
													m_timerTracker;

		EventManager&								mf_accessEventManager();

		void										mf_execute(InternalCgiWorker& worker, InternalCgiRequestData& data, bool markFdsAsStale);

		// recycle (re-use immediately)
		void										mf_recycleSuccess(InternalCgiWorker& worker);
		void										mf_recycleRuntimeFailure(InternalCgiWorker& worker);
		void										mf_recycleStartupFailure(InternalCgiWorker& worker, bool markFdsAsStale);
		void										mf_recycleTimeoutFailure(InternalCgiWorker& worker);
		void										mf_recycleExecutionUnit(InternalCgiWorker& worker, bool markFdsAsStale, e_CgiCallback callUser);
		void										mf_cancelAndRecycle(InternalCgiRequestData& data, bool markFdsAsStale);
		
		void										mf_recycleWorker(InternalCgiWorker& worker, bool markFdsAsStale);
		void										mf_recycleRequestData(InternalCgiRequestData& data);
		

		// return
		void										mf_returnExecutionUnit(InternalCgiWorker& worker, bool markFdsAsStale, e_CgiCallback callUser);
		void										mf_returnWorker(InternalCgiWorker& worker);
		void										mf_returnRequestData(InternalCgiRequestData& data);
		void										mf_cancelAndReturn(InternalCgiRequestData& data);

		int											mf_finishTimedOut();
		

		CgiModule(const CgiModule &copy);
		CgiModule &operator=(const CgiModule &assign);

};



#endif