

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

			// user sets callbacks for sucess, timeout...
			// not mandatory, if user doesn't set, it doesn't get called
			void						setNotify_Callback	(const Cgi::Notify::Type type, 
															const Cgi::Notify::Callback handler);

			// MANDATORY, the CgiModule will call this such that the user controls its own reading/Writing
			void						setIO_Callback		(const Cgi::IO::Type type, 
															const Cgi::IO::Callback handler);
			// user setter for cgi environment variables
			void						setEnvBase			(const Cgi::Env::Enum::Type env, 
															const Cgi::EnvValue& value);
			// extra variables (http specific variables, for example)												
			void						setEnvExtra			(const Cgi::EnvKey& key, 
															const Cgi::EnvValue& value);
			
			// user must pass interpreter OR extension. Module will use Path if available, and fallback to extension + internal config
			void						setInterpreterPath	(const Cgi::InterpPath& path);
			void						setExtension		(const Cgi::InterpExtension& extension);

			// script path and timeout
			void						setScriptPath		(const Cgi::ScriptPath& path);
			void						setTimeoutMs		(const unsigned int timeoutMs);

			// runtime options to delay reading writing, not mandatory to use
			void						setRuntimeOptions	(const Cgi::Options::Mask options);		

			const Cgi::EnvVariables&	getEnvVars() const;
			const Cgi::InterpPath&		getInterpreterPath() const;
			const Cgi::InterpExtension&	getExtension() const;
			const Cgi::ScriptPath&		getScriptPath() const;
			unsigned int				getTimeoutMs() const;

		protected:

			Cgi::User								m_user;
			Cgi::IO::Callback						m_IO_Handlers[Cgi::IO::COUNT];
			Cgi::Notify::Callback					m_runtime_Handlers[Cgi::Notify::COUNT];

			unsigned int							m_timeoutMs;
			Cgi::InterpPath							m_interpreterPath;
			Cgi::InterpExtension					m_extension;
			std::string								m_scriptPath;
			Cgi::EnvVariables						m_env;

			Cgi::RequestState::Type					m_state;
			Cgi::Options::Mask						m_options;

			void									mf_reset();
	};
};

#endif