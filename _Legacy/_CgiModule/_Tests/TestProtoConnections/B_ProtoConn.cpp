/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_ProtoConn.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 12:11:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "B_ProtoConn.hpp"
#include "../../../Event/Event.hpp"

void (*B_ProtoConn_CgiGateway::eventHandlers[E_CGI_CALLBACK_COUNT])(Event& event) = {
	B_ProtoConn_CgiGateway::onRead,
	B_ProtoConn_CgiGateway::onWrite,
	B_ProtoConn_CgiGateway::onError,
	B_ProtoConn_CgiGateway::onClose,
	B_ProtoConn_CgiGateway::onTimeout
};

void B_ProtoConn_CgiGateway::onRead(Event& event)
{
	B_ProtoConn& request = *static_cast<B_ProtoConn*>(event.getData());
	B_ProtoConn_CgiGateway::CgiOnRead(request);
}

void B_ProtoConn_CgiGateway::onWrite(Event& event)
{
	B_ProtoConn& request = *static_cast<B_ProtoConn*>(event.getData());
	B_ProtoConn_CgiGateway::CgiOnWrite(request);
}

void B_ProtoConn_CgiGateway::onError(Event& event)
{
	B_ProtoConn& request = *static_cast<B_ProtoConn*>(event.getData());
	B_ProtoConn_CgiGateway::CgiOnError(request);
}

void B_ProtoConn_CgiGateway::onClose(Event& event)
{
	B_ProtoConn& request = *static_cast<B_ProtoConn*>(event.getData());
	B_ProtoConn_CgiGateway::CgiOnClose(request);
}

void B_ProtoConn_CgiGateway::onTimeout(Event& event)
{
	B_ProtoConn& request = *static_cast<B_ProtoConn*>(event.getData());
	B_ProtoConn_CgiGateway::CgiOnTimeout(request);
}

void B_ProtoConn_CgiGateway::CgiOnRead(B_ProtoConn& request)
{
	//std::cout << "User read callback\n";
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
}

void B_ProtoConn_CgiGateway::CgiOnWrite(B_ProtoConn& request)
{
	//std::cout << "User write callback\n";
	(void)request;
	/* probably nothing to do */
}

void B_ProtoConn_CgiGateway::CgiOnError(B_ProtoConn& request)
{
	//std::cout << "User error callback\n";
	(void)request;
	/* send internal server error to client */
}

void B_ProtoConn_CgiGateway::CgiOnClose(B_ProtoConn& request)
{
	//std::cout << "User close callback\n";
	(void)request;
	/* all good, probably nothing to do*/
}

void B_ProtoConn_CgiGateway::CgiOnTimeout(B_ProtoConn& request)
{
	//std::cout << "User timeout callback\n";
	(void)request;
	/* send internal server error to client, either script is too long or some other bottleneck */
}