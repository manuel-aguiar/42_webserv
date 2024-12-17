/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCgiGateway.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 08:44:37 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 14:45:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCGIGATEWAY_CPP

# define HTTPCGIGATEWAY_CPP

// Project headers
# include "../../CgiModule/Cgi_Definitions.h"

//forward declarations
class Event;
class HttpRequest;

class HttpCgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onRead(Event& event);
		static void onWrite(Event& event);
		static void onError(Event& event);
		static void onClose(Event& event);
		static void onTimeout(Event& event);

		static void (*eventHandlers[E_CGI_EVENT_COUNT])(Event& event);

		// Implementation of events
		static void CgiOnRead(HttpRequest& request);
		static void CgiOnWrite(HttpRequest& request);
		static void CgiOnError(HttpRequest& request);
		static void CgiOnClose(HttpRequest& request);
		static void CgiOnTimeout(HttpRequest& request);
};



#endif