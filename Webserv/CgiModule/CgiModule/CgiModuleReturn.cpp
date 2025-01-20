

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"


namespace Cgi
{

void
Module::mf_returnWorker(Worker& worker)
{
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void
Module::mf_returnRequestData(InternalRequest& data)
{
	mf_recycleRequestData(data);
}

void
Module::mf_cancelAndReturn(InternalRequest& data)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_returnExecutionUnit(*worker, false, Runtime_Callback::ON_ERROR_RUNTIME);
}

void
Module::mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, const Runtime_Callback::Type callUser)
{
	InternalRequest* 		data = worker.accessRequestData();
	User 					user = data->getUser();
	Runtime_Callback::Handler	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	mf_returnWorker(worker);
	mf_returnRequestData(*data);
	if (user && handler)
		(handler)(user);
}

}; // namespace Cgi