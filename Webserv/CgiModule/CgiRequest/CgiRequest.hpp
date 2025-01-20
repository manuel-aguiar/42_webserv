

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

//Project Headers
# include "../CgiNamespace.h"
# include "../CgiModule/CgiModule.hpp"

// C++ headers
# include <string>

namespace Cgi
{
	class Module::Request
	{
		public:
		
			void				setUser(Module::User user);
			void				setUserCallback(const Module::CallbackType type, const Module::Callback handler);
			void				setReadHandler(Module::IO_Handler handler);
			void				setWriteHandler(Module::IO_Handler handler);
			void				setEnvBase(const Cgi::Env::Enum::Type env, const Module::EnvValue& value);
			void				setEnvExtra(const Module::EnvKey& key, const Module::EnvValue& value);
			void				setExtension(const Module::InterpExtension& extension);
			void				setScriptPath(const std::string& path);
			void				setTimeoutMs(const unsigned int timeoutMs);
			void				setRuntimeOptions(Module::RuntimeOptions options);		

			const EnvVariables& getEnvVars() const;
			const std::string&	getExtension() const;
			const std::string&	getScriptPath() const;
			unsigned int		getTimeoutMs() const;

		protected:
			Request();
			~Request();
			Request(const Request &copy);
			Request &operator=(const Request &assign);

			Module::User				m_user;
			Module::IO_Handler		m_readHandler;
			Module::IO_Handler		m_writeHandler;
			Module::Callback 		m_userCallbacks[CALLBACK_COUNT];

			unsigned int				m_timeoutMs;
			Module::InterpExtension		
										m_extension;
			std::string					m_scriptPath;
			Module::EnvVariables		m_env;

			Module::RequestState		m_state;
			Module::RuntimeOptions	m_options;

			void						mf_reset();
	};
	typedef Module::Request Request;
};

#endif