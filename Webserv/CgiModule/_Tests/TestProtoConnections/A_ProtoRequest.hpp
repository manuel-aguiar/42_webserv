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
#include "../../CgiRequestData/CgiRequestData.hpp"
#include "../../CgiModule/CgiModule.hpp"

class EventManager;
class Globals;
class CgiModule;

class A_ProtoRequest
{
    public:
		A_ProtoRequest(EventManager& manager, Globals& globals, CgiModule& cgi);
		~A_ProtoRequest();
		A_ProtoRequest(const A_ProtoRequest& copy);
		

		// Callbacks for the event manager
		void	OnRead();
		void	onWrite();

		// Callbacks for the CGI Module
		void   	executeCgi();		//On execute
		void	falseStartCgi();	// on error startup
		void	cancelCgi();		// on error runtime

		void	debugPrint() const;


		//event callbacks
		static void EventCallbackOnRead(Callback& event);
		static void EventCallbackOnWrite(Callback& event);

		void	printBufStdout();

		EventManager&	m_manager;
		Globals&		m_globals;
		CgiModule&		m_cgi;
		
		Event 			m_CgiReadEvent;
		Event 			m_CgiWriteEvent;

		CgiRequestData*	m_CgiRequestData;

		std::string		m_msgBody;
		
		char 			m_buffer[1024];
		size_t			m_TotalBytesRead;

		size_t			m_CancelCount;
};

class A_ProtoRequest_CgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onExecute(Callback& Callback);
		static void onErrorStartup(Callback& Callback);
		static void onErrorRuntime(Callback& callback);

		static void (*Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback);
};




#endif