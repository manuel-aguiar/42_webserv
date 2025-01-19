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

#ifndef TESTPROTOREQUEST_HPP

# define TESTPROTOREQUEST_HPP


#include "../../Cgi_Definitions.h"
#include "../../../Callback/Callback.hpp"
#include "../../CgiRequest/CgiRequest.hpp"
#include "../../CgiModule/CgiModule.hpp"

class EventManager;
class Globals;
class CgiModule;

class TestProtoRequest
{
    public:
		TestProtoRequest(Globals& globals, CgiModule& cgi, int id);
		~TestProtoRequest();
		TestProtoRequest(const TestProtoRequest& copy);
		

		// Callbacks for the CGI Module
		void   	SuccessCgi();		//On execute
		void	falseStartCgi();	// on error startup
		void	cancelCgi();		// on error runtime
		void	timeoutCgi();

		CgiModule::BytesRead		CgiRead(int readFd);
		CgiModule::BytesWritten		CgiWrite(int writeFd);

		void	debugPrint() const;

		enum e_CgiResultStatus
		{
			E_CGI_STATUS_WORKING,
			E_CGI_STATUS_FAILED_ACQUIRE,
			E_CGI_STATUS_SUCCESS,
			E_CGI_STATUS_ERROR_STARTUP,
			E_CGI_STATUS_ERROR_RUNTIME,
			E_CGI_STATUS_TIMEOUT,
			E_CGI_STATUS_COUNT,
		};

		void	printBufStdout();

		Globals&			m_globals;
		CgiModule&			m_cgi;
		
		CgiModule::Request*	m_CgiRequestData;

		std::string			m_msgBody;
		
		char 				m_buffer[1024];
		size_t				m_TotalBytesRead;

		int					m_id;

		std::string			m_ExpectedOutput;

		e_CgiResultStatus	m_CgiResultStatus;
};

class TestProtoRequest_CgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onSuccess(CgiModule::User user);
		static void onErrorStartup(CgiModule::User user);
		static void onErrorRuntime(CgiModule::User user);
		static void onErrorTimeOut(CgiModule::User user);

		static CgiModule::BytesRead 		onRead(CgiModule::User user, int readFd);
		static CgiModule::BytesWritten 		onWrite(CgiModule::User user, int writeFd);

		static void (*Callbacks[CgiModule::CALLBACK_COUNT])(CgiModule::User user);
};




#endif