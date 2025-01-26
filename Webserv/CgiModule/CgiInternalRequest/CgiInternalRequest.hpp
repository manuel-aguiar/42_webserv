

#ifndef CGIINTERNALREQUEST_HPP

# define CGIINTERNALREQUEST_HPP

// Project Headers
# include "../CgiRequest/CgiRequest.hpp"

class Worker;
class Timer;
namespace Cgi { class Request; }

class InternalRequest : public Cgi::Request
{
	public:
		InternalRequest();
		~InternalRequest();

		void						reset();
		void						Runtime_CallTheUser(const CgiNotify::Type type);
		IO::BytesCount				IO_CallTheUser(const CgiIO::Type, Ws::fd readFd);

		CgiUser						getUser() const;
		CgiNotify::Callback
									getRuntime_Handler(const CgiNotify::Type type) const;
		CgiModule::ImplModule::RequestState::Type
									getState() const;
		const EnvVariables& 		getEnvVars() const;
		const CgiInterpExtension&			getExtension() const;
		const std::string&			getScriptPath() const;
		unsigned int				getTimeoutMs() const;
		Worker*						accessExecutor();
		TimerTracker<Timer, InternalRequest*>::iterator
									getMyTimer() const;
		CgiOptions::Mask			getOptions() const;

		void						assignExecutor(Worker& executor);
		void						setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer);
		void						setState(const CgiModule::ImplModule::RequestState::Type state);
		
	private:
		Worker*				m_executor;
		
		TimerTracker<Timer, InternalRequest*>::iterator	
									m_myTimer;
		
		InternalRequest(const InternalRequest &copy);
		InternalRequest &operator=(const InternalRequest &assign);
};


#endif