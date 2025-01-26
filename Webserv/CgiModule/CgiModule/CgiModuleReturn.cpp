

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"


void
ImplModule::mf_returnWorker(Worker& worker)
{
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void
ImplModule::mf_returnRequestData(InternalRequest& data)
{
	mf_recycleRequestData(data);
}

void
ImplModule::mf_cancelAndReturn(InternalRequest& data)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_returnExecutionUnit(*worker, false, Cgi::Notify::ON_ERROR_RUNTIME);
}

void
ImplModule::mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, const Cgi::Notify::Type callUser)
{
	InternalRequest* 		data = worker.accessRequestData();
	Cgi::User 				user = data->getUser();
	Cgi::Notify::Callback	handler = data->getRuntime_Handler(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	mf_returnWorker(worker);
	mf_returnRequestData(*data);
	if (user && handler)
		(handler)(user);
}