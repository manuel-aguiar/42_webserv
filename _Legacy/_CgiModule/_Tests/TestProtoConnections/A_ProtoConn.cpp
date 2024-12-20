/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 09:15:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A_ProtoRequest.hpp"
#include "../../../Event/Event.hpp"

void (*A_ProtoRequest_CgiGateway::eventHandlers[E_CGI_EVENT_COUNT])(Event& event) = {
	A_ProtoRequest_CgiGateway::onRead,
	A_ProtoRequest_CgiGateway::onWrite,
	A_ProtoRequest_CgiGateway::onError,
	A_ProtoRequest_CgiGateway::onClose,
	A_ProtoRequest_CgiGateway::onTimeout
};

void A_ProtoRequest_CgiGateway::onRead(Event& event)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(event.getData());
	A_ProtoRequest_CgiGateway::CgiOnRead(request);
}

void A_ProtoRequest_CgiGateway::onWrite(Event& event)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(event.getData());
	A_ProtoRequest_CgiGateway::CgiOnWrite(request);
}

void A_ProtoRequest_CgiGateway::onError(Event& event)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(event.getData());
	A_ProtoRequest_CgiGateway::CgiOnError(request);
}

void A_ProtoRequest_CgiGateway::onClose(Event& event)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(event.getData());
	A_ProtoRequest_CgiGateway::CgiOnClose(request);
}

void A_ProtoRequest_CgiGateway::onTimeout(Event& event)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(event.getData());
	A_ProtoRequest_CgiGateway::CgiOnTimeout(request);
}

void A_ProtoRequest_CgiGateway::CgiOnRead(A_ProtoRequest& request)
{
	//std::cout << "User read callback\n";
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
}

void A_ProtoRequest_CgiGateway::CgiOnWrite(A_ProtoRequest& request)
{
	//std::cout << "User write callback\n";
	(void)request;
	/* probably nothing to do */
}

void A_ProtoRequest_CgiGateway::CgiOnError(A_ProtoRequest& request)
{
	//std::cout << "User error callback\n";
	(void)request;
	/* send internal server error to client */
}

void A_ProtoRequest_CgiGateway::CgiOnClose(A_ProtoRequest& request)
{
	//std::cout << "User close callback\n";
	(void)request;
	/* all good, probably nothing to do*/
}

void A_ProtoRequest_CgiGateway::CgiOnTimeout(A_ProtoRequest& request)
{
	//std::cout << "User timeout callback\n";
	(void)request;
	/* send internal server error to client, either script is too long or some other bottleneck */
}