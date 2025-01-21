

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
		
			void					setUser				(const CgiUser user);
			void					setRuntime_Callback	(const CgiRuntime_Callback::Type type, 
														const CgiRuntime_Callback::Handler handler);
			void					setIO_Callback		(const CgiIO_Callback::Type type, 
														const CgiIO_Callback::Handler handler);
			void					setEnvBase			(const CgiEnvEnum::Type env, const CgiEnvValue& value);
			void					setEnvExtra			(const CgiEnvKey& key, const CgiEnvValue& value);
			void					setExtension		(const CgiInterpExtension& extension);
			void					setScriptPath		(const CgiScriptPath& path);
			void					setTimeoutMs		(const unsigned int timeoutMs);
			void					setRuntimeOptions	(const CgiOptions::Flags options);		

			const CgiEnvVariables&	getEnvVars() const;
			const std::string&		getExtension() const;
			const std::string&		getScriptPath() const;
			unsigned int			getTimeoutMs() const;

		protected:
			Request();
			~Request();
			Request(const Request &copy);
			Request &operator=(const Request &assign);

			CgiUser								m_user;
			CgiIO_Callback::Handler				m_IO_Handlers[CgiIO_Callback::COUNT];
			CgiRuntime_Callback::Handler		m_runtime_Handlers[CgiRuntime_Callback::COUNT];

			unsigned int						m_timeoutMs;
			CgiInterpExtension					m_extension;
			std::string							m_scriptPath;
			CgiEnvVariables						m_env;

			Cgi::Module::RequestState::Type		m_state;
			CgiOptions::Flags					m_options;

			void								mf_reset();
	};
	typedef Cgi::Module::Request Request;
};

#endif