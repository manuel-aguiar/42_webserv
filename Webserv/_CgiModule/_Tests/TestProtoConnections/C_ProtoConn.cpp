/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_ProtoConn.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 09:16:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "C_ProtoConn.hpp"
#include "../../../Event/Event.hpp"

void (*C_ProtoConn_CgiGateway::eventHandlers[E_CGI_EVENT_COUNT])(Event& event) = {
	C_ProtoConn_CgiGateway::onRead,
	C_ProtoConn_CgiGateway::onWrite,
	C_ProtoConn_CgiGateway::onError,
	C_ProtoConn_CgiGateway::onClose,
	C_ProtoConn_CgiGateway::onTimeout
};

void C_ProtoConn_CgiGateway::onRead(Event& event)
{
	C_ProtoConn& request = *static_cast<C_ProtoConn*>(event.getData());
	C_ProtoConn_CgiGateway::CgiOnRead(request);
}

void C_ProtoConn_CgiGateway::onWrite(Event& event)
{
	C_ProtoConn& request = *static_cast<C_ProtoConn*>(event.getData());
	C_ProtoConn_CgiGateway::CgiOnWrite(request);
}

void C_ProtoConn_CgiGateway::onError(Event& event)
{
	C_ProtoConn& request = *static_cast<C_ProtoConn*>(event.getData());
	C_ProtoConn_CgiGateway::CgiOnError(request);
}

void C_ProtoConn_CgiGateway::onClose(Event& event)
{
	C_ProtoConn& request = *static_cast<C_ProtoConn*>(event.getData());
	C_ProtoConn_CgiGateway::CgiOnClose(request);
}

void C_ProtoConn_CgiGateway::onTimeout(Event& event)
{
	C_ProtoConn& request = *static_cast<C_ProtoConn*>(event.getData());
	C_ProtoConn_CgiGateway::CgiOnTimeout(request);
}

void C_ProtoConn_CgiGateway::CgiOnRead(C_ProtoConn& request)
{
	//std::cout << "User read callback\n";
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
}

void C_ProtoConn_CgiGateway::CgiOnWrite(C_ProtoConn& request)
{
	//std::cout << "User write callback\n";
	(void)request;
	/* probably nothing to do */
}

void C_ProtoConn_CgiGateway::CgiOnError(C_ProtoConn& request)
{
	//std::cout << "User error callback\n";
	(void)request;
	/* send internal server error to client */
}

void C_ProtoConn_CgiGateway::CgiOnClose(C_ProtoConn& request)
{
	//std::cout << "User close callback\n";
	(void)request;
	/* all good, probably nothing to do*/
}

void C_ProtoConn_CgiGateway::CgiOnTimeout(C_ProtoConn& request)
{
	//std::cout << "User timeout callback\n";
	(void)request;
	/* send internal server error to client, either script is too long or some other bottleneck */
}