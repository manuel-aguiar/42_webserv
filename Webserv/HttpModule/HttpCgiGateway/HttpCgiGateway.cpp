/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCgiGateway.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 08:41:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 14:45:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "HttpCgiGateway.hpp"
#include "../HttpRequest/HttpRequest.hpp"
#include "../../Event/Event.hpp"

void (*HttpCgiGateway::eventHandlers[E_CGI_EVENT_COUNT])(Event& event) = {
	HttpCgiGateway::onRead,
	HttpCgiGateway::onWrite,
	HttpCgiGateway::onError,
	HttpCgiGateway::onClose,
	HttpCgiGateway::onTimeout
};

void HttpCgiGateway::onRead(Event& event)
{
	HttpRequest& request = *static_cast<HttpRequest*>(event.getData());
	HttpCgiGateway::CgiOnRead(request);
}

void HttpCgiGateway::onWrite(Event& event)
{
	HttpRequest& request = *static_cast<HttpRequest*>(event.getData());
	HttpCgiGateway::CgiOnWrite(request);
}

void HttpCgiGateway::onError(Event& event)
{
	HttpRequest& request = *static_cast<HttpRequest*>(event.getData());
	HttpCgiGateway::CgiOnError(request);
}

void HttpCgiGateway::onClose(Event& event)
{
	HttpRequest& request = *static_cast<HttpRequest*>(event.getData());
	HttpCgiGateway::CgiOnClose(request);
}

void HttpCgiGateway::onTimeout(Event& event)
{
	HttpRequest& request = *static_cast<HttpRequest*>(event.getData());
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