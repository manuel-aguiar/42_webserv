/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleInternal.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:52:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 14:02:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"


void	CgiModule::mf_returnWorker(InternalCgiWorker& worker)
{
	InternalCgiRequestData* requestData;

	requestData = static_cast<InternalCgiRequestData*>(worker.accessCurRequestData());
	
	if (requestData)
		mf_returnRequestData(*requestData);

    worker.reset();

	if (m_pendingRequests.size() > 0)
	{
		worker.execute(*m_pendingRequests.front());
		m_pendingRequests.pop_front();
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