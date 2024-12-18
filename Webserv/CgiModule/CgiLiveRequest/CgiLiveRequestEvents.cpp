/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequestEvents.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:27:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 09:24:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiLiveRequest.hpp"

void	CgiModule::CgiLiveRequest::mf_CgiWrite(Event& event)
{
	CgiLiveRequest* request;
	
	request = static_cast<CgiLiveRequest*>(event.getData());
	request->writeToChild();
}

void	CgiModule::CgiLiveRequest::mf_CgiRead(Event& event)
{
	CgiLiveRequest*	request;
	
	request = static_cast<CgiLiveRequest*>(event.getData());
	request->readFromChild();
}