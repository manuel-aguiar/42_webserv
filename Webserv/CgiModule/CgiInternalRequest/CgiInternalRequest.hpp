

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
			void						Runtime_CallTheUser(const Module::Runtime_Callback::Type type);
			IO_Callback::BytesCount		IO_CallTheUser(const Module::IO_Callback::Type, Ws::fd readFd);

			Module::User				getUser() const;
			Module::Runtime_Callback::Handler
										getRuntime_Handler(const Module::Runtime_Callback::Type type) const;
			Module::RequestState::Type	getState() const;
			const EnvVariables& 		getEnvVars() const;
			const std::string&			getExtension() const;
			const std::string&			getScriptPath() const;
			unsigned int				getTimeoutMs() const;
			Worker*						accessExecutor();
			TimerTracker<Timer, InternalRequest*>::iterator
										getMyTimer() const;
			Module::Options::Flags		getOptions() const;

			void						assignExecutor(Worker& executor);
			void						setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer);
			void						setState(const RequestState::Type state);
			
		private:
			Worker*						m_executor;
			
			TimerTracker<Timer, InternalRequest*>::iterator	
										m_myTimer;
			
			InternalRequest(const InternalRequest &copy);
			InternalRequest &operator=(const InternalRequest &assign);
	};
};



#endif