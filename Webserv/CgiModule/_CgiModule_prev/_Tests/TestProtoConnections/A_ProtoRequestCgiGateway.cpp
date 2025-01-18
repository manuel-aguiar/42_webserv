/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequestCgiGateway.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:51:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 10:13:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "A_ProtoRequest.hpp"
# include "../../Test_EventManager/Test_EventManager.hpp"

void (*A_ProtoRequest_CgiGateway::Callbacks[E_CGI_CALLBACK_COUNT])(CgiRequestData::t_ptr_CgiUser User) = {
	A_ProtoRequest_CgiGateway::onSuccess,
	A_ProtoRequest_CgiGateway::onErrorStartup,
	A_ProtoRequest_CgiGateway::onErrorRuntime,
	A_ProtoRequest_CgiGateway::onErrorTimeOut,
};

void A_ProtoRequest_CgiGateway::onSuccess(CgiRequestData::t_ptr_CgiUser user)
{
	reinterpret_cast<A_ProtoRequest*>(user)->SuccessCgi();
}

void A_ProtoRequest_CgiGateway::onErrorStartup(CgiRequestData::t_ptr_CgiUser user)
{
	reinterpret_cast<A_ProtoRequest*>(user)->falseStartCgi();
}

void A_ProtoRequest_CgiGateway::onErrorRuntime(CgiRequestData::t_ptr_CgiUser user)
{
	reinterpret_cast<A_ProtoRequest*>(user)->cancelCgi();
}

void	A_ProtoRequest_CgiGateway::onErrorTimeOut(CgiRequestData::t_ptr_CgiUser user)
{
	reinterpret_cast<A_ProtoRequest*>(user)->timeoutCgi();
}


CgiRequestData::t_bytesRead		A_ProtoRequest_CgiGateway::onWrite(CgiRequestData::t_ptr_CgiUser user, int writeFd)
{
	return (reinterpret_cast<A_ProtoRequest*>(user)->newCgiWrite(writeFd));
}

CgiRequestData::t_bytesWritten	A_ProtoRequest_CgiGateway::onRead(CgiRequestData::t_ptr_CgiUser user, int readFd)
{
	return (reinterpret_cast<A_ProtoRequest*>(user)->newCgiRead(readFd));
}

