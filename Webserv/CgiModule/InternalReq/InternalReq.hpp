

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
		Cgi::User					getUser() const;
		Cgi::RequestState::Type		getState() const;
		
		Cgi::Notify::Callback		getNotifyOnError_Callback() const;
		Cgi::Notify::Callback		getNotifyOnSuccess_Callback() const;

		Cgi::Notify::Callback		getRuntime_Handler(const Cgi::Notify::Type callUser) const;

		Cgi::IO::FillCgiWriteBuffer	getWriteToScript_Callback() const;
		Cgi::IO::Callback			getReceiveScriptBody_Callback() const;

		Cgi::IO::ReceiveStatusHeaders	getReceiveHeaders_Callback() const;

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