/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestDependencies.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 14:55:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TestDependencies.hpp"
#include "../../Event/Event.hpp"

void (*CgiUserGateway::eventHandlers[E_CGI_EVENT_COUNT])(Event& event) = {
	CgiUserGateway::onRead,
	CgiUserGateway::onWrite,
	CgiUserGateway::onError,
	CgiUserGateway::onClose,
	CgiUserGateway::onTimeout
};

void CgiUserGateway::onRead(Event& event)
{
	CgiUser& request = *static_cast<CgiUser*>(event.getData());
	CgiUserGateway::CgiOnRead(request);
}

void CgiUserGateway::onWrite(Event& event)
{
	CgiUser& request = *static_cast<CgiUser*>(event.getData());
	CgiUserGateway::CgiOnWrite(request);
}

void CgiUserGateway::onError(Event& event)
{
	CgiUser& request = *static_cast<CgiUser*>(event.getData());
	CgiUserGateway::CgiOnError(request);
}

void CgiUserGateway::onClose(Event& event)
{
	CgiUser& request = *static_cast<CgiUser*>(event.getData());
	CgiUserGateway::CgiOnClose(request);
}

void CgiUserGateway::onTimeout(Event& event)
{
	CgiUser& request = *static_cast<CgiUser*>(event.getData());
	CgiUserGateway::CgiOnTimeout(request);
}

void CgiUserGateway::CgiOnRead(CgiUser& request)
{
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
}

void CgiUserGateway::CgiOnWrite(CgiUser& request)
{
	(void)request;
	/* probably nothing to do */
}

void CgiUserGateway::CgiOnError(CgiUser& request)
{
	(void)request;
	/* send internal server error to client */
}

void CgiUserGateway::CgiOnClose(CgiUser& request)
{
	(void)request;
	/* all good, probably nothing to do*/
}

void CgiUserGateway::CgiOnTimeout(CgiUser& request)
{
	(void)request;
	/* send internal server error to client, either script is too long or some other bottleneck */
}