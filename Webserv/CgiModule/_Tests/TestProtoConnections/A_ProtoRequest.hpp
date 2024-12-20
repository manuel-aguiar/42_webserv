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
		
		void	readCgi();
		void	writeCgi();
		void   	executeCgi();
		void	cancelCgi();

		void	requestCgi();

		void	printBufStdout();


		EventManager&	m_manager;
		Globals&		m_globals;
		CgiModule&		m_cgi;

		CgiRequestData*	m_curRequestData;

		std::string		m_msgBody;
		
		char 			m_buffer[1024];
};

class A_ProtoRequest_CgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onExecute(Callback& Callback);
		static void onErrorStartup(Callback& Callback);
		static void onErrorRuntime(Callback& callback);

		static void (*Callbacks[E_CGI_CALLBACK_COUNT])(Callback& Callback);

		// Implementation of Callbacks
		static void CgiOnExecute(A_ProtoRequest& request);
		static void CgiOnErrorStartup(A_ProtoRequest& request);
		static void CgiOnErrorRuntime(A_ProtoRequest& request);

};




#endif