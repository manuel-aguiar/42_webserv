/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 10:08:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A_ProtoRequest.hpp"
#include "../../../Callback/Callback.hpp"


A_ProtoRequest::A_ProtoRequest(EventManager& manager, Globals& globals) :
	manager(manager),
	globals(globals)
{

}

A_ProtoRequest::~A_ProtoRequest()
{
}




void (*A_ProtoRequest_CgiGateway::Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback) = {
	A_ProtoRequest_CgiGateway::onExecute,
	A_ProtoRequest_CgiGateway::onRead,
	A_ProtoRequest_CgiGateway::onWrite,
	A_ProtoRequest_CgiGateway::onError,
	A_ProtoRequest_CgiGateway::onClose,
	A_ProtoRequest_CgiGateway::onTimeout
};

void A_ProtoRequest_CgiGateway::onExecute(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnExecute(request);
}

void A_ProtoRequest_CgiGateway::onRead(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnRead(request);
}

void A_ProtoRequest_CgiGateway::onWrite(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnWrite(request);
}

void A_ProtoRequest_CgiGateway::onError(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnError(request);
}

void A_ProtoRequest_CgiGateway::onClose(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnClose(request);
}

void A_ProtoRequest_CgiGateway::onTimeout(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnTimeout(request);
}

void A_ProtoRequest_CgiGateway::CgiOnExecute(A_ProtoRequest& request)
{
	//std::cout << "User read callback\n";
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
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