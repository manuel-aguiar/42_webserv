/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorker.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:42:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 11:14:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef InternalCgiWorker_HPP

# define InternalCgiWorker_HPP

//Project Headers
# include "../Cgi_Definitions.h"
# include "../CgiModule/CgiModule.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Event/Event.hpp"

class EventManager;
class CgiRequestData;
class Globals;
class Connection;

// implement some timeout for script execution, if nothing then kill

class CgiModule::InternalCgiWorker
{
	public:
		InternalCgiWorker(CgiModule& manager, Globals& globals);
		~InternalCgiWorker();
		InternalCgiWorker(const InternalCgiWorker &other);
		InternalCgiWorker &operator=(const InternalCgiWorker &other);


		void    					execute();
		void    					reset();
		
		void						stop();
		
		void						disableAllEvents(bool markAsStale = true);
		
		void						assignRequestData(InternalCgiRequestData& data);
		InternalCgiRequestData*		accessRequestData();



	private:

		InternalCgiRequestData*		m_curRequestData;

		// scripts arguments
		DynArray<std::string>		m_envStr;
		DynArray<char *>			m_envPtr;
		DynArray<char *>			m_argPtr;

		// pipes and buffers
		t_fd						m_ParentToChild[2];
		t_fd						m_ChildToParent[2];
		t_fd						m_EmergencyPhone[2];
		char						m_EmergencyBuffer[2];
		int							m_EmergencyBytesRead;
		Event						m_EmergencyEvent;
		t_pid						m_pid;

		Event						m_readEvent;
		Event						m_writeEvent;

		typedef enum
		{
			E_EMER_DUP2 = 1,
			E_EMER_EXECVE,
		} 	e_EmergencyCode;

		
		// External sources
		CgiModule&					m_CgiModule;
		Globals&					m_globals;

		// execute
		void						mf_executeParent();
		void						mf_executeChild();
		bool						mf_prepareExecve();

		// Events
		static void					mf_EventCallback_OnEmergency(Callback& event);	
		void						mf_disableMyEvent(Event& myEvent, bool markAsStale = true);
		void						mf_readEmergencyPhone();
		void						mf_childFailure();

		static void					mf_EventCallback_onRead(Callback& callback);
		static void					mf_EventCallback_onWrite(Callback& callback);

		void						mf_readScript();
		void						mf_writeScript();


		// Other helpers
		void						mf_closeFd(t_fd& fd);
		void						mf_waitChild();
		void						mf_KillWaitChild();


		void						mf_closeAllPipes();



};



#endif
