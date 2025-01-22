

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"


namespace Cgi
{

	void
	Cgi::Module::mf_returnWorker(Worker& worker)
	{
		m_availableWorkers.push_back(&worker);
		m_busyWorkerCount--;
	}

	void
	Cgi::Module::mf_returnRequestData(InternalRequest& data)
	{
		mf_recycleRequestData(data);
	}

	void
	Cgi::Module::mf_cancelAndReturn(InternalRequest& data)
	{
		Worker*		worker = data.accessExecutor();
		
		worker->stop();
		mf_returnExecutionUnit(*worker, false, CgiRuntime_Callback::ON_ERROR_RUNTIME);
	}

	void
	Cgi::Module::mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, const CgiRuntime_Callback::Type callUser)
	{
		InternalRequest* 		data = worker.accessRequestData();
		User 					user = data->getUser();
		CgiRuntime_Callback::Service	handler = data->getRuntime_Handler(callUser);

		worker.disableCloseAllEvents(markFdsAsStale);
		mf_returnWorker(worker);
		mf_returnRequestData(*data);
		if (user && handler)
			(handler)(user);
	}

}; // namespace Cgi