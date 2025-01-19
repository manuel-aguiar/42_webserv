

#ifndef CGIWORKER_HPP

# define CGIWORKER_HPP

//Project Headers
# include "../Cgi_Definitions.h"
# include "../CgiModule/CgiModule.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Event/Event.hpp"

class Request;
class Globals;
class Connection;

// implement some timeout for script execution, if nothing then kill

class CgiModule::Worker
{
	public:
		Worker(CgiModule& cgi);
		~Worker();

		void    	execute(bool markFdsAsStale);
		void    	reset();
		
		void		stop();
		
		void		disableReadHandler();
		void		disableWriteHandler();
		void		disableEmergencyHandler();
		void		disableAllHandlers();

		void		enableReadHandler();
		void		enableWriteHandler();
		void		enableEmergencyHandler();
		void		enableAllHandlers();

		void		disableReadEvent(bool markAsStale);
		void		disableWriteEvent(bool markAsStale);
		void		disableEmergencyEvent(bool markAsStale);
		void		disableCloseAllEvents(bool markAsStale);
		
		void		assignRequestData(InternalRequest& data);
		InternalRequest*
					accessRequestData();



	private:

		InternalRequest*	m_curRequestData;

		// script arguments
		DynArray<std::string>
							m_envStr;
		DynArray<char *>	m_envPtr;
		DynArray<char *>	m_argPtr;

		// pipes and buffers
		t_fd				m_ParentToChild[2];
		t_fd				m_ChildToParent[2];
		t_fd				m_EmergencyPhone[2];
		char				m_EmergencyBuffer[2];
		int					m_EmergencyBytesRead;
		Event				m_EmergencyEvent;
		t_pid				m_pid;

		Event				m_readEvent;
		Event				m_writeEvent;

		typedef enum
		{
			E_EMER_DUP2 = 1,
			E_EMER_EXECVE,
		} 	e_EmergencyCode;

		
		// External sources
		CgiModule&			m_CgiModule;

		// execute
		void				mf_executeParent(bool markFdsAsStale);
		void				mf_executeChild();
		bool				mf_prepareExecve();

		// Events
		static void			mf_EventCallback_OnEmergency(::Callback& event);	
		static void			mf_EventCallback_onRead(::Callback& callback);
		static void			mf_EventCallback_onWrite(::Callback& callback);

		// Event Handlers
		void				mf_readEmergencyPhone();
		void				mf_readScript();
		void				mf_writeScript();
		void				mf_disableCloseMyEvent(::Event& myEvent, bool markAsStale = true);

		// Other helpers
		void				mf_closeFd(t_fd& fd);
		void				mf_childFailure();
		void				mf_waitChild();
		void				mf_KillWaitChild();

		//private as usual
		Worker(const Worker &other);
		Worker &operator=(const Worker &other);
};



#endif
