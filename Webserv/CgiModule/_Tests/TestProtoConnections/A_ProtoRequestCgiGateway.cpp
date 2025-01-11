/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequestCgiGateway.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:51:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 13:18:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../../ServerManager/EventManager/EventManager.hpp"

void (*A_ProtoRequest_CgiGateway::Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback) = {
	A_ProtoRequest_CgiGateway::onExecute,
	A_ProtoRequest_CgiGateway::onErrorStartup,
	A_ProtoRequest_CgiGateway::onErrorRuntime,
	A_ProtoRequest_CgiGateway::onErrorTimeOut,
};

void A_ProtoRequest_CgiGateway::onExecute(Callback& callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(callback.getData());
	request.executeCgi();
}

void A_ProtoRequest_CgiGateway::onErrorStartup(Callback& callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(callback.getData());
	request.falseStartCgi();
}

void A_ProtoRequest_CgiGateway::onErrorRuntime(Callback& callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(callback.getData());
	request.cancelCgi();
}

void	A_ProtoRequest_CgiGateway::onErrorTimeOut(Callback& callback)
{
	A_ProtoRequest& request = *static_cast<A_ProtoRequest*>(callback.getData());
	request.timeoutCgi();
}

