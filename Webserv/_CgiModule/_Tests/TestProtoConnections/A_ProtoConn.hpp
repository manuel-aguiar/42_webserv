/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:09:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 09:16:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_ProtoRequest_HPP

# define A_ProtoRequest_HPP


#include "../../Cgi_Definitions.h"

class Event;

class A_ProtoRequest
{
    
};

class A_ProtoRequest_CgiGateway
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
		static void CgiOnRead(A_ProtoRequest& request);
		static void CgiOnWrite(A_ProtoRequest& request);
		static void CgiOnError(A_ProtoRequest& request);
		static void CgiOnClose(A_ProtoRequest& request);
		static void CgiOnTimeout(A_ProtoRequest& request);	
};




#endif