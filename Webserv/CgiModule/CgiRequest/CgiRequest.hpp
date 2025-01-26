

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

//Project Headers
# include "../m_CgiNamespace.h"

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
		
			void					setUser				(const Cgi::User user);
			void					setNotify_Callback	(const Cgi::Notify::Type type, 
														const Cgi::Notify::Callback handler);
			void					setIO_Callback		(const Cgi::IO::Type type, 
														const Cgi::IO::Callback handler);
			void					setEnvBase			(const Cgi::Env::Enum::Type env, const Cgi::EnvValue& value);
			void					setEnvExtra			(const Cgi::EnvKey& key, const Cgi::EnvValue& value);
			void					setExtension		(const Cgi::InterpExtension& extension);
			void					setScriptPath		(const Cgi::ScriptPath& path);
			void					setTimeoutMs		(const unsigned int timeoutMs);
			void					setRuntimeOptions	(const Cgi::Options::Mask options);		

			const Cgi::EnvVariables&	getEnvVars() const;
			const Cgi::InterpExtension&	getExtension() const;
			const Cgi::ScriptPath&		getScriptPath() const;
			unsigned int				getTimeoutMs() const;

		protected:

			Cgi::User								m_user;
			Cgi::IO::Callback						m_IO_Handlers[Cgi::IO::COUNT];
			Cgi::Notify::Callback					m_runtime_Handlers[Cgi::Notify::COUNT];

			unsigned int							m_timeoutMs;
			Cgi::InterpExtension					m_extension;
			std::string								m_scriptPath;
			Cgi::EnvVariables						m_env;

			Cgi::RequestState::Type					m_state;
			Cgi::Options::Mask						m_options;

			void								mf_reset();
	};
};

#endif