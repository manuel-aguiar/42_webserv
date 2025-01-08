/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 14:56:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"

void	CgiModule::mf_execute(InternalCgiWorker& worker, InternalCgiRequestData& data)
{
	data.setState(InternalCgiRequestData::E_CGI_STATE_EXECUTING);
	data.setExecutor(&worker);
	worker.execute(data);
}

void	CgiModule::mf_returnWorker(InternalCgiWorker& worker)
{
	InternalCgiRequestData* currentData;
    InternalCgiRequestData* newData;


	currentData = static_cast<InternalCgiRequestData*>(worker.accessCurRequestData());
	
	if (currentData)
		mf_returnRequestData(*currentData);

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
        mf_returnRequestData(*newData);
	}
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void	CgiModule::mf_returnRequestData(InternalCgiRequestData& data)
{	
	data.reset();
	m_availableRequestData.push_back(&data);
}