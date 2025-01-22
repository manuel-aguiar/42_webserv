

#ifndef CGIWORKER_HPP

# define CGIWORKER_HPP

//Project Headers
# include "../CgiModule/CgiModule.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Events/Subscription/Subscription.hpp"

// implement some timeout for script execution, if nothing then kill
namespace Cgi
{
	class Module::Worker
	{
		public:
			Worker(Module& cgi);
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
			
			void		assignRequestData(Module::InternalRequest& data);
			Module::InternalRequest*
						accessRequestData();



		private:

			InternalRequest*			m_curRequestData;

			// script arguments
			DynArray<std::string>
										m_envStr;
			DynArray<char *>			m_envPtr;
			DynArray<char *>			m_argPtr;

			// pipes and buffers
			Ws::fd						m_ParentToChild[2];
			Ws::fd						m_ChildToParent[2];
			Ws::fd						m_EmergencyPhone[2];
			char						m_EmergencyBuffer[2];
			int							m_EmergencyBytesRead;
			Events::Subscription*		m_EmergencyEvent;
			t_pid						m_pid;

			Events::Subscription*		m_readEvent;
			Events::Subscription*		m_writeEvent;

			typedef enum
			{
				E_EMER_DUP2 = 1,
				E_EMER_EXECVE,
			} 	e_EmergencyCode;

			
			// External sources
			Cgi::Module&		m_CgiModule;

			// execute
			void				mf_executeParent(bool markFdsAsStale);
			void				mf_executeChild();
			bool				mf_prepareExecve();

			// Events
			static void			mf_EventCallback_OnEmergency(Events::Subscription& event);	
			static void			mf_EventCallback_onRead(Events::Subscription& event);
			static void			mf_EventCallback_onWrite(Events::Subscription& event);

			// Callback Handlers
			void				mf_readEmergencyPhone();
			void				mf_readScript();
			void				mf_writeScript();
			void				mf_disableCloseMyEvent(Events::Subscription& myEvent, bool markAsStale = true);

			// Other helpers
			void				mf_closeFd(Ws::fd& fd);
			void				mf_childFailure();
			void				mf_waitChild();
			void				mf_KillWaitChild();

			//private as usual
			Worker(const Worker &other);
			Worker &operator=(const Worker &other);
	};
};


#endif
