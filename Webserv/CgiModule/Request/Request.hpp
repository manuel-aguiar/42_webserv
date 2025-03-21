

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

//Project Headers
# include "../CgiNamespace.h"

// C++ headers
# include <string>

namespace Cgi
{
	class Request
	{
		public:
			Request();
			~Request();
			Request(const Request &copy);
			Request &operator=(const Request &assign);

			// object that will be called back in case of IO or Notify events
			void						setUser				(const Cgi::User user);

			// not mandatory, if user doesn't set, it doesn't get called
			void						setNotify_onError	(const Cgi::Notify::Callback handler);
			void						setNotify_onSuccess	(const Cgi::Notify::Callback handler);
			void						setWriteToScript_Callback
															(const Cgi::IO::OnReadWrite handler);
			void						setReadBodyFromScript_Callback
															(const Cgi::IO::OnReadWrite handler);
			void						setReceiveStatusHeaders_Callback
															(const Cgi::IO::ReceiveHeaderData handler);

			// user setter for cgi environment variables
			void						setEnvBase			(const Cgi::Env::Enum::Type env, 
															const Cgi::EnvValue& value);
			// extra variables (http specific variables, for example)												
			void						setEnvExtra			(const Cgi::EnvKey& key, 
															const Cgi::EnvValue& value);
			
			// user must pass interpreter OR extension. Module will use Path if available, and fallback to extension + internal config
			void						setInterpreterPath	(const Cgi::InterpPath& path);

			// script path and timeout
			void						setScriptPath		(const Cgi::ScriptPath& path);
			void						setTimeoutMs		(const unsigned int timeoutMs);

			// runtime options to delay reading writing, not mandatory to use
			void						setRuntimeOptions	(const Cgi::Options::Mask options);		

			const Cgi::EnvVariables&	getEnvVars() const;
			const Cgi::InterpPath&		getInterpreterPath() const;
			const Cgi::ScriptPath&		getScriptPath() const;
			
			unsigned int				getTimeoutMs() const;

		protected:
			
			// callbacks
			Cgi::User								m_user;
			Cgi::Notify::Callback					m_notifyOnError;
			Cgi::Notify::Callback					m_notifyOnSuccess;

			Cgi::IO::OnReadWrite					m_writeToScript;
			Cgi::IO::ReceiveHeaderData				m_receiveStatusHeaders;
			Cgi::IO::OnReadWrite					m_readBodyFromScript;
			
			// actual state of the request
			unsigned int							m_timeoutMs;
			Cgi::InterpPath							m_interpreterPath;
			std::string								m_scriptPath;
			Cgi::EnvVariables						m_env;

			Cgi::RequestState::Type					m_state;
			Cgi::Options::Mask						m_options;

			void									mf_reset();

	};
}

#endif
