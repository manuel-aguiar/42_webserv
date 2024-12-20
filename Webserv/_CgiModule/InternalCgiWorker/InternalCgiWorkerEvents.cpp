/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerEvents.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:27:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 17:05:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InternalCgiWorker.hpp"

void	CgiModule::InternalCgiWorker::mf_CgiWrite(Event& event)
{
	InternalCgiWorker* request;
	
	request = static_cast<InternalCgiWorker*>(event.getData());
	request->writeToChild();
}

void	CgiModule::InternalCgiWorker::mf_CgiRead(Event& event)
{
	InternalCgiWorker*	request;
	
	request = static_cast<InternalCgiWorker*>(event.getData());
	request->readFromChild();
}