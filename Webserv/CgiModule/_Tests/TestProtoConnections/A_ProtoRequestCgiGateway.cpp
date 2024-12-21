/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequestCgiGateway.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:51:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/21 01:02:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../../ServerManager/EventManager/EventManager.hpp"

void (*A_ProtoRequest_CgiGateway::Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback) = {
	A_ProtoRequest_CgiGateway::onExecute,
	A_ProtoRequest_CgiGateway::onErrorStartup,
	A_ProtoRequest_CgiGateway::onErrorRuntime,
};

void A_ProtoRequest_CgiGateway::onExecute(Callback& callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(callback.getData());
	request.executeCgi();
}

void A_ProtoRequest_CgiGateway::onErrorStartup(Callback& callback)
{
	(void)callback;
}

void A_ProtoRequest_CgiGateway::onErrorRuntime(Callback& callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(callback.getData());
	request.cancelCgi();
}

