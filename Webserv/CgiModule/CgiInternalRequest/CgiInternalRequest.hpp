

#ifndef CGIINTERNALREQUEST_HPP

# define CGIINTERNALREQUEST_HPP

// Project Headers
# include "../CgiModule/CgiModule.hpp"
# include "../CgiRequest/CgiRequest.hpp"

namespace Cgi
{
	class Module::InternalRequest : public Module::Request
	{
		public:
			InternalRequest();
			~InternalRequest();

			void						reset();
			void						Runtime_CallTheUser(const CgiRuntime_Callback::Type type);
			IO_Callback::BytesCount		IO_CallTheUser(const CgiIO_Callback::Type, Ws::fd readFd);

			CgiUser						getUser() const;
			CgiRuntime_Callback::Service
										getRuntime_Handler(const CgiRuntime_Callback::Type type) const;
			CgiModule::Module::RequestState::Type
										getState() const;
			const EnvVariables& 		getEnvVars() const;
			const CgiInterpExtension&			getExtension() const;
			const std::string&			getScriptPath() const;
			unsigned int				getTimeoutMs() const;
			mt_CgiWorker*				accessExecutor();
			TimerTracker<Timer, InternalRequest*>::iterator
										getMyTimer() const;
			CgiOptions::Monitor			getOptions() const;

			void						assignExecutor(Worker& executor);
			void						setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer);
			void						setState(const CgiModule::Module::RequestState::Type state);
			
		private:
			mt_CgiWorker*				m_executor;
			
			TimerTracker<Timer, InternalRequest*>::iterator	
										m_myTimer;
			
			InternalRequest(const InternalRequest &copy);
			InternalRequest &operator=(const InternalRequest &assign);
	};
};



#endif