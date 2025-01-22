

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"

namespace Cgi
{

	void
	Cgi::Module::mf_recycleWorker(Worker& worker, bool markFdsAsStale)
	{
		InternalRequest* newData;

		worker.reset();

		while (m_executionQueue.size() > 0)
		{
			newData = m_executionQueue.front();
			m_executionQueue.pop_front();

			if (newData->getState() != RequestState::CANCELLED)
			{
				mf_execute(worker, *newData, markFdsAsStale);
				return ;
			}
			mf_recycleRequestData(*newData);
		}
		m_availableWorkers.push_back(&worker);
		m_busyWorkerCount--;
	}

	void
	Cgi::Module::mf_recycleRequestData(InternalRequest& data)
	{	
		TimerTracker<Timer, InternalRequest*>::iterator 	timer;

		timer = data.getMyTimer();
		if (timer != TimerTracker<Timer, InternalRequest*>::iterator())
			m_timerTracker.erase(timer);

		data.reset();

		m_availableRequestData.push_back(&data);
	}

	void
	Cgi::Module::mf_recycleSuccess(Worker& worker)
	{
		mf_recycleExecutionUnit(worker, true, CgiNotify::ON_SUCCESS);
	}


	void
	Cgi::Module::mf_recycleRuntimeFailure(Worker& worker)
	{
		worker.stop();
		mf_recycleExecutionUnit(worker, true, CgiNotify::ON_ERROR_RUNTIME);
	}

	void
	Cgi::Module::mf_recycleStartupFailure(Worker& worker, bool markFdsAsStale)
	{
		mf_recycleExecutionUnit(worker, markFdsAsStale, CgiNotify::ON_ERROR_STARTUP);
	}

	void
	Cgi::Module::mf_recycleTimeoutFailure(Worker& worker)
	{
		worker.stop();
		mf_recycleExecutionUnit(worker, false, CgiNotify::ON_ERROR_TIMEOUT);
	}

	void
	Cgi::Module::mf_cancelAndRecycle(InternalRequest& data, bool markFdsAsStale)
	{
		Worker*		worker = data.accessExecutor();
		
		worker->stop();
		mf_recycleExecutionUnit(*worker, markFdsAsStale, CgiNotify::ON_ERROR_RUNTIME);
	}

	void
	Cgi::Module::mf_recycleExecutionUnit(Worker& worker, bool markFdsAsStale, const CgiNotify::Type callUser)
	{
		InternalRequest* 			data = worker.accessRequestData();
		User 						user = data->getUser();
		CgiNotify::Callback	handler = data->getRuntime_Handler(callUser);

		worker.disableCloseAllEvents(markFdsAsStale);
		mf_recycleWorker(worker, markFdsAsStale);
		mf_recycleRequestData(*data);
		if (user && handler)
			(handler)(user);
	}

}; // namespace Cgi