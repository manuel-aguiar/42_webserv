/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiModuleReturn.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:51:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 09:51:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../InternalCgiWorker/InternalCgiWorker.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "CgiModule.hpp"


void	CgiModule::mf_returnWorker(InternalCgiWorker& worker)
{
	m_availableWorkers.push_back(&worker);
	m_busyWorkerCount--;
}

void	CgiModule::mf_returnRequestData(InternalCgiRequestData& data)
{
	mf_recycleRequestData(data);
}

void		CgiModule::mf_cancelAndReturn(InternalCgiRequestData& data)
{
	InternalCgiWorker*		worker = data.accessExecutor();
	
	worker->stop();
	mf_returnExecutionUnit(*worker, false, E_CGI_ON_ERROR_RUNTIME);
}

void		CgiModule::mf_returnExecutionUnit(InternalCgiWorker& worker, bool markFdsAsStale, e_CgiCallback callUser)
{
	InternalCgiRequestData* 					data = worker.accessRequestData();
	InternalCgiRequestData::t_ptr_CgiUser 		user = data->getUser();
	InternalCgiRequestData::t_func_CgiHandler 	handler = data->getHandler(callUser);

	worker.disableAllEvents(markFdsAsStale);
	mf_returnWorker(worker);
	mf_returnRequestData(*data);
	if (user && handler)
		(handler)(user);
}