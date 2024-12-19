/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 14:15:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"


void	CgiModule::mf_returnWorker(InternalCgiWorker& worker)
{
	InternalCgiRequestData* currentData;
    InternalCgiRequestData* newData;


	currentData = static_cast<InternalCgiRequestData*>(worker.accessCurRequestData());
	
	if (currentData)
		mf_returnRequestData(*currentData);

    worker.reset();

	if (m_pendingRequests.size() > 0)
	{
        newData = m_pendingRequests.front();
        m_pendingRequests.pop_front();
        newData->setPendingLocation(NULL);
        
		worker.execute(*newData);
	}
	else
	{
		m_availableCgiWorkers.push_back(&worker);
		m_busyWorkerCount--;
	}
}

void	CgiModule::mf_returnRequestData(InternalCgiRequestData& data)
{	
    List<InternalCgiRequestData*, MPool_FixedElem<InternalCgiRequestData*> >::iterator pendingIter;

    pendingIter = data.accessPendingLocation();

    if (pendingIter != NULL)
		m_pendingRequests.erase(pendingIter);
	data.reset();
	m_availableRequestData.push_back(&data);
}