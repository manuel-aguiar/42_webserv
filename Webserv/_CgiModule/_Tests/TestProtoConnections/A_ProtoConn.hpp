/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoConn.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:09:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 09:16:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_PROTOCONN_HPP

# define A_PROTOCONN_HPP


#include "../../Cgi_Definitions.h"

class Event;

class A_ProtoConn
{
    
};

class A_ProtoConn_CgiGateway
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
		static void CgiOnRead(A_ProtoConn& request);
		static void CgiOnWrite(A_ProtoConn& request);
		static void CgiOnError(A_ProtoConn& request);
		static void CgiOnClose(A_ProtoConn& request);
		static void CgiOnTimeout(A_ProtoConn& request);	
};




#endif