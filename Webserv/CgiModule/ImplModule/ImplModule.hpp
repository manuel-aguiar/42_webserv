

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

// Toolkit Headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/StackArray/StackArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

// Project Headers
# include "../CgiNamespace.h"
# include "../../TimerTracker/TimerTracker.hpp"

// C++ headers
# include <map>

// forward declarations
class Globals;
class Worker;
class InternalReq;
class Timer;
namespace Cgi { class Request;}
namespace Events {class Manager;}

class ImplModule
{
	protected:
		ImplModule(	const size_t workerCount, 
					const size_t backlogCount, 
					const size_t maxTimeout, 
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

		enum { MIN_TIMEOUT = 1};



		// helper functions the Worker will need to use to inform the manager about execution
		void				_WorkerRecycleSuccess(Worker& worker);
		void				_WorkerRecycleRuntimeFailure(Worker& worker);
		void				_WorkerRecycleStartupFailure(Worker& worker, bool markFdsAsStale);

		Events::Manager&	_WorkerAccessEventManager();
		Globals&			_WorkerAccessGlobals();

	private:

		size_t									m_numWorkers;
		size_t									m_backlog;
		size_t									m_maxTimeout;
		size_t									m_busyWorkerCount;

		HeapArray<Worker>						m_allWorkers;
		HeapArray<InternalReq>					m_allRequestData;
		
		HeapCircularQueue<Worker*>				m_availableWorkers;
		HeapCircularQueue<InternalReq*>			m_availableRequestData;
		HeapCircularQueue<InternalReq*>			m_executionQueue;

		std::map<Cgi::InterpExtension, Cgi::InterpPath>
												m_Interpreters;

		StackArray<Cgi::EnvKey,Cgi::Env::Enum::COUNT>
												m_baseEnv;

		TimerTracker<Timer, InternalReq*>		m_timerTracker;
		Events::Manager&						m_eventManager;
		Globals&								m_globals;


		// internal helper functions
		void				mf_execute(Worker& worker, InternalReq& data, bool markFdsAsStale);
		int					mf_finishTimedOut();

		void				mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser);
		void				mf_returnWorker(Worker& worker);
		void				mf_returnRequestData(InternalReq& data);
		void				mf_cancelAndReturn(InternalReq& data);
		
		void				mf_recycleTimeoutFailure(Worker& worker);
		void				mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser);
		void				mf_cancelAndRecycle(InternalReq& data, bool markFdsAsStale);
		void				mf_recyclePendingFinish(InternalReq& data, bool markFdsAsStale);

		void				mf_recycleWorker(Worker& worker, bool markFdsAsStale);
		void				mf_recycleRequestData(InternalReq& data);
		
		// private copy/assignment
		ImplModule(const ImplModule &copy);
		ImplModule &operator=(const ImplModule &assign);
};

#endif
