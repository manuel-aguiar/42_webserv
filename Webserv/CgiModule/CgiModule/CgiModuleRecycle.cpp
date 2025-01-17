/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleRecycle.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:50:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 11:24:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

void	CgiModule::mf_recycleWorker(InternalCgiWorker& worker)
{
    InternalCgiRequestData* newData;

    worker.reset();

	while (m_executionQueue.size() > 0)
	{
        newData = m_executionQueue.front();
        m_executionQueue.pop_front();

		if (newData->getState() != InternalCgiRequestData::E_CGI_STATE_CANCELLED)
		{
			mf_execute(worker, *newData);
			return ;
		}
        mf_recycleRequestData(*newData);
	}
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void	CgiModule::mf_recycleRequestData(InternalCgiRequestData& data)
{	
	TimerTracker<Timer, InternalCgiRequestData*>::iterator 	timer;

	timer = data.getMyTimer();
	if (timer != TimerTracker<Timer, InternalCgiRequestData*>::iterator())
		m_timerTracker.erase(timer);

	data.reset();

	m_availableRequestData.push_back(&data);
}

void		CgiModule::mf_recycleSuccess(InternalCgiWorker& worker)
{
	mf_recycleExecutionUnit(worker, true, E_CGI_ON_SUCCESS);
}


void		CgiModule::mf_recycleRuntimeFailure(InternalCgiWorker& worker)
{
	worker.stop();
	mf_recycleExecutionUnit(worker, true, E_CGI_ON_ERROR_RUNTIME);
}

void		CgiModule::mf_recycleStartupFailure(InternalCgiWorker& worker)
{
	mf_recycleExecutionUnit(worker, false, E_CGI_ON_ERROR_STARTUP);
}

void		CgiModule::mf_recycleTimeoutFailure(InternalCgiWorker& worker)
{
	mf_recycleExecutionUnit(worker, false, E_CGI_ON_ERROR_TIMEOUT);
}

void	CgiModule::mf_cancelAndRecycle(InternalCgiRequestData& data)
{
	InternalCgiWorker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_recycleExecutionUnit(*worker, true, E_CGI_ON_ERROR_RUNTIME);
}

void		CgiModule::mf_recycleExecutionUnit(InternalCgiWorker& worker, bool markFdsAsStale, e_CgiCallback callUser)
{
	InternalCgiRequestData* 					data = worker.accessRequestData();
	InternalCgiRequestData::t_ptr_CgiUser 		user = data->getUser();
	InternalCgiRequestData::t_func_CgiHandler 	handler = data->getHandler(callUser);

	worker.disableAllEvents(markFdsAsStale);
	mf_recycleWorker(worker);
	mf_recycleRequestData(*data);
	if (user && handler)
		(handler)(user);
}