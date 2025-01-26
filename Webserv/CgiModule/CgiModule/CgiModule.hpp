

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

// Toolkit Headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/StackArray/StackArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

// Project Headers
# include "../m_CgiNamespace.h"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Events/Events.h"
# include "../../TimerTracker/TimerTracker.hpp"

// C++ headers
# include <map>

// forward declarations
class Globals;
class Worker;
class InternalRequest;
class Timer;
namespace Cgi { class Request;}

class ImplModule
{
	protected:
		ImplModule(	size_t workerCount, 
					size_t backlogCount, 
					size_t maxTimeout, 
					Events::Manager& eventManager, 
					Globals& globals);
		~ImplModule();

	public:

		// request interaction
		Cgi::Request*		acquireRequest();
		void				enqueueRequest(Cgi::Request& data, bool isCalledFromEventLoop);
		void				modifyRequest(Cgi::Request& data, bool isCalledFromEventLoop, Cgi::Options::Mask newOptions);
		void				finishRequest(Cgi::Request& data, bool isCalledFromEventLoop);
		
		// processing
		int					processRequests();
		void				stopAndReset();

		// configuring interpreters
		void				addInterpreter(const Cgi::InterpExtension& extension, const Cgi::InterpPath& path);
		void				removeInterpreter(const Cgi::InterpExtension& extension);

		// getters
		size_t				getBusyWorkerCount() const;
		size_t				getQueueSize() const;
		const StackArray<Cgi::EnvKey, Cgi::Env::Enum::COUNT>&
							getBaseEnvKeys() const;
		const std::map<Cgi::InterpExtension, Cgi::InterpPath>&	
							getInterpreters() const;


		#define						CGI_MIN_TIMEOUT 1



		//helper functions that will be called by the helper classes, mostly
		// will be HIDDEN FROM THE PUBLIC INTERFACE
		void				mf_execute(Worker& worker, InternalRequest& data, bool markFdsAsStale);

		// recycle (re-use immediately)
		void				mf_recycleSuccess(Worker& worker);
		void				mf_recycleRuntimeFailure(Worker& worker);
		void				mf_recycleStartupFailure(Worker& worker, bool markFdsAsStale);
		void				mf_recycleTimeoutFailure(Worker& worker);
		void				mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser);
		void				mf_cancelAndRecycle(InternalRequest& data, bool markFdsAsStale);
		
		void				mf_recycleWorker(Worker& worker, bool markFdsAsStale);
		void				mf_recycleRequestData(InternalRequest& data);
		
		// return
		void				mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser);
		void				mf_returnWorker(Worker& worker);
		void				mf_returnRequestData(InternalRequest& data);
		void				mf_cancelAndReturn(InternalRequest& data);

		Events::Manager&	mf_accessEventManager();
		Globals&			mf_accessGlobals();

	private:

		size_t									m_numWorkers;
		size_t									m_backlog;
		size_t									m_maxTimeout;
		size_t									m_busyWorkerCount;

		HeapArray<Worker>						m_allWorkers;
		HeapArray<InternalRequest>				m_allRequestData;
		
		HeapCircularQueue<Worker*>				m_availableWorkers;
		HeapCircularQueue<InternalRequest*>		m_availableRequestData;
		HeapCircularQueue<InternalRequest*>		m_executionQueue;

		std::map<Cgi::InterpExtension, Cgi::InterpPath>
												m_Interpreters;

		StackArray<Cgi::EnvKey,Cgi::Env::Enum::COUNT>
												m_baseEnv;

		TimerTracker<Timer, InternalRequest*>	m_timerTracker;
		Events::Manager&						m_eventManager;
		Globals&								m_globals;

		int										mf_finishTimedOut();
		
		// private copy/assignment
		ImplModule(const ImplModule &copy);
		ImplModule &operator=(const ImplModule &assign);
};


#endif