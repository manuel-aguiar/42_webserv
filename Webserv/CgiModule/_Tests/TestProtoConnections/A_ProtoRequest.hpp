/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ProtoRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:09:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 10:18:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_ProtoRequest_HPP

# define A_ProtoRequest_HPP


#include "../../Cgi_Definitions.h"
#include "../../../Callback/Callback.hpp"

class EventManager;
class Globals;

class A_ProtoRequest
{
    public:
		A_ProtoRequest(EventManager& manager, Globals& globals);
		~A_ProtoRequest();
		

	private:
		EventManager&	manager;
		Globals&		globals;	
		
		char 			buffer[1024];
};

class A_ProtoRequest_CgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onExecute(Callback& Callback);
		static void onRead(Callback& Callback);
		static void onWrite(Callback& Callback);
		static void onError(Callback& Callback);
		static void onClose(Callback& Callback);
		static void onTimeout(Callback& Callback);

		static void (*Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback);

		// Implementation of Callbacks
		static void CgiOnExecute(A_ProtoRequest& request);
		static void CgiOnRead(A_ProtoRequest& request);
		static void CgiOnWrite(A_ProtoRequest& request);
		static void CgiOnError(A_ProtoRequest& request);
		static void CgiOnClose(A_ProtoRequest& request);
		static void CgiOnTimeout(A_ProtoRequest& request);	
};




#endif