/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_ProtoConn.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:09:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 09:15:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PROTOCONN_HPP

# define C_PROTOCONN_HPP


#include "../../Cgi_Definitions.h"

class Event;

class C_ProtoConn
{
    
};

class C_ProtoConn_CgiGateway
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
		static void CgiOnRead(C_ProtoConn& request);
		static void CgiOnWrite(C_ProtoConn& request);
		static void CgiOnError(C_ProtoConn& request);
		static void CgiOnClose(C_ProtoConn& request);
		static void CgiOnTimeout(C_ProtoConn& request);	
};




#endif