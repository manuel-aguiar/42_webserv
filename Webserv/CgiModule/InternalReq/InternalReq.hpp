

#ifndef CGIINTERNALREQUEST_HPP

# define CGIINTERNALREQUEST_HPP

// Project Headers
# include "../Request/Request.hpp"
# include "../../TimerTracker/TimerTracker.hpp"

class Worker;
class Timer;
namespace Ws { typedef int fd; }

class InternalReq : public Cgi::Request
{
	public:
		InternalReq();
		~InternalReq();

		void						reset();
		void						Runtime_CallTheUser(const Cgi::Notify::Type type);
		Cgi::IO::BytesCount			IO_CallTheUser(const Cgi::IO::Type, Ws::fd readFd);

		Cgi::User					getUser() const;
		Cgi::Notify::Callback
									getRuntime_Handler(const Cgi::Notify::Type type) const;
		Cgi::RequestState::Type
									getState() const;
		
		Cgi::IO::ReceiveStatusHeaders	getDelieverHeadersCallback();

		const Cgi::EnvVariables& 	getEnvVars() const;
		const Cgi::InterpExtension&	getExtension() const;
		const std::string&			getScriptPath() const;
		unsigned int				getTimeoutMs() const;
		Worker*						accessExecutor();

		TimerTracker<Timer, InternalReq*>::iterator
									getMyTimer() const;
		Cgi::Options::Mask			getOptions() const;

		void						assignExecutor(Worker& executor);
		void						setMyTimer(const TimerTracker<Timer, InternalReq*>::iterator& timer);
		void						setState(const Cgi::RequestState::Type state);
		
	private:
		Worker*						m_executor;
		
		TimerTracker<Timer, InternalReq*>::iterator	
									m_myTimer;
		
		InternalReq(const InternalReq &copy);
		InternalReq &operator=(const InternalReq &assign);
};


#endif