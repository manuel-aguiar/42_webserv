/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleReturn.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:51:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/19 14:20:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../CgiWorker/CgiWorker.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "CgiModule.hpp"


void
CgiModule::mf_returnWorker(Worker& worker)
{
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void
CgiModule::mf_returnRequestData(InternalRequest& data)
{
	mf_recycleRequestData(data);
}

void
CgiModule::mf_cancelAndReturn(InternalRequest& data)
{
	Worker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_returnExecutionUnit(*worker, false, CALLBACK_ON_ERROR_RUNTIME);
}

void
CgiModule::mf_returnExecutionUnit(Worker& worker, bool markFdsAsStale, CgiModule::CallbackType callUser)
{
	InternalRequest* 	data = worker.accessRequestData();
	User 				user = data->getUser();
	Callback		 	handler = data->getUserCallback(callUser);

	worker.disableCloseAllEvents(markFdsAsStale);
	mf_returnWorker(worker);
	mf_returnRequestData(*data);
	if (user && handler)
		(handler)(user);
}