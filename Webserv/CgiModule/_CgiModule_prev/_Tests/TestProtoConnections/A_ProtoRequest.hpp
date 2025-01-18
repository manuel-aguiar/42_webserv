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
#include "../../../../Callback/Callback.hpp"
#include "../../CgiRequestData/CgiRequestData.hpp"
#include "../../CgiModule/CgiModule.hpp"

class EventManager;
class Globals;
class CgiModule;

class A_ProtoRequest
{
    public:
		A_ProtoRequest(Globals& globals, CgiModule& cgi, int id);
		~A_ProtoRequest();
		A_ProtoRequest(const A_ProtoRequest& copy);
		

		// Callbacks for the CGI Module
		void   	SuccessCgi();		//On execute
		void	falseStartCgi();	// on error startup
		void	cancelCgi();		// on error runtime
		void	timeoutCgi();

		CgiRequestData::t_bytesRead		newCgiRead(int readFd);
		CgiRequestData::t_bytesWritten	newCgiWrite(int writeFd);

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

		Globals&		m_globals;
		CgiModule&		m_cgi;
		
		CgiRequestData*	m_CgiRequestData;

		std::string		m_msgBody;
		
		char 			m_buffer[1024];
		size_t			m_TotalBytesRead;

		int				m_id;

		std::string		m_ExpectedOutput;

		e_CgiResultStatus	m_CgiResultStatus;
};

class A_ProtoRequest_CgiGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onSuccess(CgiRequestData::t_ptr_CgiUser user);
		static void onErrorStartup(CgiRequestData::t_ptr_CgiUser user);
		static void onErrorRuntime(CgiRequestData::t_ptr_CgiUser user);
		static void onErrorTimeOut(CgiRequestData::t_ptr_CgiUser user);

		static CgiRequestData::t_bytesRead 		onRead(CgiRequestData::t_ptr_CgiUser, int readFd);
		static CgiRequestData::t_bytesWritten 	onWrite(CgiRequestData::t_ptr_CgiUser callback, int writeFd);

		static void (*Callbacks[E_CGI_CALLBACK_COUNT])(CgiRequestData::t_ptr_CgiUser user);
};




#endif