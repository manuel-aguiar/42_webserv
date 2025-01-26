

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

//Project Headers
# include "../m_CgiNamespace.h"
# include "../CgiModule/CgiModule.hpp"

// C++ headers
# include <string>

namespace Cgi
{
	class Request
	{
		public:
		
			void					setUser				(const CgiUser user);
			void					setNotify_Callback	(const CgiNotify::Type type, 
														const CgiNotify::Callback handler);
			void					setIO_Callback		(const CgiIO::Type type, 
														const CgiIO::Callback handler);
			void					setEnvBase			(const CgiEnvEnum::Type env, const CgiEnvValue& value);
			void					setEnvExtra			(const CgiEnvKey& key, const CgiEnvValue& value);
			void					setExtension		(const CgiInterpExtension& extension);
			void					setScriptPath		(const CgiScriptPath& path);
			void					setTimeoutMs		(const unsigned int timeoutMs);
			void					setRuntimeOptions	(const CgiOptions::Mask options);		

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
			CgiIO::Callback						m_IO_Handlers[CgiIO::COUNT];
			CgiNotify::Callback					m_runtime_Handlers[CgiNotify::COUNT];

			unsigned int						m_timeoutMs;
			CgiInterpExtension					m_extension;
			std::string							m_scriptPath;
			CgiEnvVariables						m_env;

			Cgi::ImplModule::RequestState::Type		m_state;
			CgiOptions::Mask					m_options;

			void								mf_reset();
	};
};

#endif