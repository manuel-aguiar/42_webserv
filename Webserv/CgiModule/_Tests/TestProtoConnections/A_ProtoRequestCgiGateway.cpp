/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest_Gateway.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:51:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 13:29:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../../ServerManager/EventManager/EventManager.hpp"

void (*A_ProtoRequest_CgiGateway::Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback) = {
	A_ProtoRequest_CgiGateway::onExecute,
	A_ProtoRequest_CgiGateway::onErrorStartup,
	A_ProtoRequest_CgiGateway::onErrorRuntime,
};

void A_ProtoRequest_CgiGateway::onExecute(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnExecute(request);
}

void A_ProtoRequest_CgiGateway::onErrorStartup(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnErrorStartup(request);
}

void A_ProtoRequest_CgiGateway::onErrorRuntime(Callback& Callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(Callback.getData());
	A_ProtoRequest_CgiGateway::CgiOnErrorRuntime(request);
}

void A_ProtoRequest_CgiGateway::CgiOnExecute(A_ProtoRequest& request)
{
	request.executeCgi();
}

void A_ProtoRequest_CgiGateway::CgiOnErrorStartup(A_ProtoRequest& request)
{
	//std::cout << "User read callback\n";
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
}

void A_ProtoRequest_CgiGateway::CgiOnErrorRuntime(A_ProtoRequest& request)
{
	request.cancelCgi();
}
