/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCgiGateway.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 08:41:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/21 12:49:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "HttpCgiGateway.hpp"
#include "../HttpRequest/HttpRequest.hpp"
#include "../../Callback/Callback.hpp"

void (*HttpCgiGateway::Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback) = {
	HttpCgiGateway::onRead,
	HttpCgiGateway::onWrite,
	HttpCgiGateway::onError
};


void HttpCgiGateway::onRead(Callback& Callback)
{
	HttpRequest& request = *static_cast<HttpRequest*>(Callback.getData());
	HttpCgiGateway::CgiOnRead(request);
}

void HttpCgiGateway::onWrite(Callback& Callback)
{
	HttpRequest& request = *static_cast<HttpRequest*>(Callback.getData());
	HttpCgiGateway::CgiOnWrite(request);
}

void HttpCgiGateway::onError(Callback& Callback)
{
	HttpRequest& request = *static_cast<HttpRequest*>(Callback.getData());
	HttpCgiGateway::CgiOnError(request);
}

void HttpCgiGateway::onClose(Callback& Callback)
{
	HttpRequest& request = *static_cast<HttpRequest*>(Callback.getData());
	HttpCgiGateway::CgiOnClose(request);
}

void HttpCgiGateway::onTimeout(Callback& Callback)
{
	HttpRequest& request = *static_cast<HttpRequest*>(Callback.getData());
	HttpCgiGateway::CgiOnTimeout(request);
}

void HttpCgiGateway::CgiOnRead(HttpRequest& request)
{
	(void)request;
	/** received info on CgiRequestData buffer, pass to client */
}

void HttpCgiGateway::CgiOnWrite(HttpRequest& request)
{
	(void)request;
	/* probably nothing to do */
}

void HttpCgiGateway::CgiOnError(HttpRequest& request)
{
	(void)request;
	/* send internal server error to client */
}

void HttpCgiGateway::CgiOnClose(HttpRequest& request)
{
	(void)request;
	/* all good, probably nothing to do*/
}

void HttpCgiGateway::CgiOnTimeout(HttpRequest& request)
{
	(void)request;
	/* send internal server error to client, either script is too long or some other bottleneck */
}