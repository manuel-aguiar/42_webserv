

#ifndef CGIMODULE_HPP

# define CGIMODULE_HPP

// Toolkit Headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/StackArray/StackArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"
# include "../../../Toolkit/List/List.hpp"

// Project Headers
# include "../CgiNamespace.h"
# include "../../GenericUtils/Webserver_Definitions.h"

# include "../../TimerTracker/TimerTracker.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

// C++ headers
# include <map>

// forward declarations
class Globals;
class EventManager;

namespace Cgi
{
	class Module
	{
		public:

			class 						Request;

			typedef std::string			InterpExtension;
			typedef std::string			InterpPath;
			typedef int 				RuntimeOptions;
			
			Module(	size_t workerCount, 
						size_t backlogCount, 
						size_t maxTimeout, 
						EventManager& eventManager, 
						Globals& globals);
			~Module();

			// request interaction
			Request*					acquireRequest();
			void						enqueueRequest(Request& data, bool isCalledFromEventLoop);
			void						modifyRequest(Request& data, RuntimeOptions newOptions, bool isCalledFromEventLoop);
			void						finishRequest(Request& data, bool isCalledFromEventLoop);
			
			// processing
			int							processRequests();
			void						stopAndReset();

			// configuring interpreters
			void						addInterpreter(const InterpExtension& extension, const InterpPath& path);
			void						removeInterpreter(const InterpExtension& extension);

			typedef std::string			EnvKey;
			typedef std::string			EnvValue;

			typedef int					BytesRead;
			typedef int					BytesWritten;
			typedef int					BytesReadWritten;
			typedef void*				User;

			typedef void 				(*Callback)	(User user);
			typedef BytesReadWritten 	(*IO_Handler)		(User user, int targetFd);

			typedef struct s_CgiRequestEnv
			{
				DynArray<std::pair<Cgi::Env::Enum::Type, EnvValue> >	envBase;
				DynArray<std::pair<EnvKey, EnvValue> >			envExtra;
			}	EnvVariables;

			typedef enum
			{
				CALLBACK_ON_SUCCESS,
				CALLBACK_ON_ERROR_STARTUP,
				CALLBACK_ON_ERROR_RUNTIME,
				CALLBACK_ON_ERROR_TIMEOUT,
				CALLBACK_COUNT
			} 	CallbackType;

			typedef enum
			{
				RUNTIME_BASE = 0,
				HOLD_READ = (1 << 1),
				RESTART_READ = (1 << 2),
				CANCEL_READ = (1 << 3),
				HOLD_WRITE = (1 << 4),
				RESTART_WRITE = (1 << 5),
				CANCEL_WRITE = (1 << 6),
			} 	e_RuntimeOptions;


			// getters
			size_t						getBusyWorkerCount() const;
			size_t						getQueueSize() const;
			const StackArray<EnvKey, Cgi::Env::Enum::COUNT>&
										getBaseEnvKeys() const;
			const std::map<InterpExtension, InterpPath>&	
										getInterpreters() const;


			#define						CGI_MIN_TIMEOUT 1


		private:

			class 									Worker;
			class									InternalRequest;

			size_t									m_numWorkers;
			size_t									m_backlog;
			size_t									m_maxTimeout;
			size_t									m_busyWorkerCount;

			HeapArray<Worker>						m_allWorkers;
			HeapArray<InternalRequest>				m_allRequestData;
			
			HeapCircularQueue<Worker*>				m_availableWorkers;
			HeapCircularQueue<InternalRequest*>		m_availableRequestData;
			HeapCircularQueue<InternalRequest*>		m_executionQueue;

			std::map<InterpExtension, InterpPath>	m_Interpreters;

			StackArray<EnvKey,Cgi::Env::Enum::COUNT>
													m_baseEnv;

			TimerTracker<Timer, InternalRequest*>	m_timerTracker;
			EventManager&							m_eventManager;
			Globals&								m_globals;

			//enums for private coordination
			typedef enum
			{
				STATE_IDLE,
				STATE_ACQUIRED,
				STATE_QUEUED,
				STATE_EXECUTING,
				STATE_CANCELLED,
			} 	RequestState;

			EventManager&		mf_accessEventManager();
			Globals&			mf_accessGlobals();

			int					mf_finishTimedOut();
			void				mf_execute(Worker& worker, InternalRequest& data, bool markFdsAsStale);

			// recycle (re-use immediately)
			void				mf_recycleSuccess(Worker& worker);
			void				mf_recycleRuntimeFailure(Worker& worker);
			void				mf_recycleStartupFailure(Worker& worker, bool markFdsAsStale);
			void				mf_recycleTimeoutFailure(Worker& worker);
			void				mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, CallbackType callUser);
			void				mf_cancelAndRecycle(InternalRequest& data, bool markFdsAsStale);
			
			void				mf_recycleWorker(Worker& worker, bool markFdsAsStale);
			void				mf_recycleRequestData(InternalRequest& data);
			
			// return
			void				mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, CallbackType callUser);
			void				mf_returnWorker(Worker& worker);
			void				mf_returnRequestData(InternalRequest& data);
			void				mf_cancelAndReturn(InternalRequest& data);

			
			// private copy/assignment
			Module(const Module &copy);
			Module &operator=(const Module &assign);

	};
};


#endif