

#ifndef CGIINTERNALREQUEST_HPP

# define CGIINTERNALREQUEST_HPP

// Project Headers
# include "../Cgi_Definitions.h"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiRequest/CgiRequest.hpp"

class CgiModule::InternalRequest : public CgiModule::Request
{
	public:
		InternalRequest();
		~InternalRequest();

		void					reset();
		void					CallTheUser(const CallbackType type);
		BytesRead				UserRead(t_fd readFd);
		BytesWritten			UserWrite(t_fd writeFd);

		User					getUser() const;
		Callback				getUserCallback(const CallbackType type) const;
		RequestState			getState() const;
		const EnvVariables& 	getEnvVars() const;
		const std::string&		getExtension() const;
		const std::string&		getScriptPath() const;
		unsigned int			getTimeoutMs() const;
		Worker*					accessExecutor();
		TimerTracker<Timer, InternalRequest*>::iterator
								getMyTimer() const;
		RuntimeOptions					getOptions() const;

		void					assignExecutor(Worker& executor);
		void					setMyTimer(const TimerTracker<Timer, InternalRequest*>::iterator& timer);
		void					setState(const RequestState state);
		
	private:
		Worker*					m_executor;
		
		TimerTracker<Timer, InternalRequest*>::iterator	
								m_myTimer;
		
		InternalRequest(const InternalRequest &copy);
		InternalRequest &operator=(const InternalRequest &assign);
};


#endif