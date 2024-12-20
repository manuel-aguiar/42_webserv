/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoConn.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 09:15:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A_ProtoConn.hpp"
#include "../../../Event/Event.hpp"

void (*A_ProtoConn_CgiGateway::eventHandlers[E_CGI_EVENT_COUNT])(Event& event) = {
	A_ProtoConn_CgiGateway::onRead,
	A_ProtoConn_CgiGateway::onWrite,
	A_ProtoConn_CgiGateway::onError,
	A_ProtoConn_CgiGateway::onClose,
	A_ProtoConn_CgiGateway::onTimeout
};

void A_ProtoConn_CgiGateway::onRead(Event& event)
{
	A_ProtoConn& request = *static_cast<A_ProtoConn*>(event.getData());
	A_ProtoConn_CgiGateway::CgiOnRead(request);
}

void A_ProtoConn_CgiGateway::onWrite(Event& event)
{
	A_ProtoConn& request = *static_cast<A_ProtoConn*>(event.getData());
	A_ProtoConn_CgiGateway::CgiOnWrite(request);
}

void A_ProtoConn_CgiGateway::onError(Event& event)
{
	A_ProtoConn& request = *static_cast<A_ProtoConn*>(event.getData());
	A_ProtoConn_CgiGateway::CgiOnError(request);
}

void A_ProtoConn_CgiGateway::onClose(Event& event)
{
	A_ProtoConn& request = *static_cast<A_ProtoConn*>(event.getData());
	A_ProtoConn_CgiGateway::CgiOnClose(request);
}

void A_ProtoConn_CgiGateway::onTimeout(Event& event)
{
	A_ProtoConn& request = *static_cast<A_ProtoConn*>(event.getData());
	A_ProtoConn_CgiGateway::CgiOnTimeout(request);
}

void A_ProtoConn_CgiGateway::CgiOnRead(A_ProtoConn& request)
{
	//std::cout << "User read callback\n";
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
}

void A_ProtoConn_CgiGateway::CgiOnWrite(A_ProtoConn& request)
{
	//std::cout << "User write callback\n";
	(void)request;
	/* probably nothing to do */
}

void A_ProtoConn_CgiGateway::CgiOnError(A_ProtoConn& request)
{
	//std::cout << "User error callback\n";
	(void)request;
	/* send internal server error to client */
}

void A_ProtoConn_CgiGateway::CgiOnClose(A_ProtoConn& request)
{
	//std::cout << "User close callback\n";
	(void)request;
	/* all good, probably nothing to do*/
}

void A_ProtoConn_CgiGateway::CgiOnTimeout(A_ProtoConn& request)
{
	//std::cout << "User timeout callback\n";
	(void)request;
	/* send internal server error to client, either script is too long or some other bottleneck */
}