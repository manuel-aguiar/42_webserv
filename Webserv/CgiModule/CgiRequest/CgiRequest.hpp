

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
			void				setRuntime_Callback(const Module::Runtime_Callback::Type type, 
												const Module::Runtime_Callback::Handler handler);
			void				setIO_Callback(	const Module::IO_Callback::Type type, 
												const Module::IO_Callback::Handler handler);
			void				setEnvBase(const Cgi::Env::Enum::Type env, const Module::EnvValue& value);
			void				setEnvExtra(const Module::EnvKey& key, const Module::EnvValue& value);
			void				setExtension(const Module::InterpExtension& extension);
			void				setScriptPath(const std::string& path);
			void				setTimeoutMs(const unsigned int timeoutMs);
			void				setRuntimeOptions(Module::Options::Flags options);		

			const EnvVariables& getEnvVars() const;
			const std::string&	getExtension() const;
			const std::string&	getScriptPath() const;
			unsigned int		getTimeoutMs() const;

		protected:
			Request();
			~Request();
			Request(const Request &copy);
			Request &operator=(const Request &assign);

			Module::User					m_user;
			Module::IO_Callback::Handler	m_IO_Handlers[Module::IO_Callback::COUNT];
			Module::Runtime_Callback::Handler	m_runtime_Handlers[Module::Runtime_Callback::COUNT];

			unsigned int					m_timeoutMs;
			Module::InterpExtension		
											m_extension;
			std::string						m_scriptPath;
			Module::EnvVariables			m_env;

			Module::RequestState::Type		m_state;
			Module::Options::Flags			m_options;

			void							mf_reset();
	};
	typedef Module::Request Request;
};

#endif