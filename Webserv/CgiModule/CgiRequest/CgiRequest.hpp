

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

//Project Headers
# include "../Cgi_Definitions.h"
# include "../CgiModule/CgiModule.hpp"

// C++ headers
# include <string>

class CgiModule::Request
{
	public:
	
		void				setUser(CgiModule::User user);
		void				setUserCallback(const CgiModule::CallbackType type, const CgiModule::Callback handler);
		void				setReadHandler(CgiModule::IO_Handler handler);
		void				setWriteHandler(CgiModule::IO_Handler handler);
		void				setEnvBase(const Cgi::EnvType env, const CgiModule::EnvValue& value);
		void				setEnvExtra(const CgiModule::EnvKey& key, const CgiModule::EnvValue& value);
		void				setExtension(const CgiModule::InterpExtension& extension);
		void				setScriptPath(const std::string& path);
		void				setTimeoutMs(const unsigned int timeoutMs);
		void				setRuntimeOptions(CgiModule::RuntimeOptions options);		

		const EnvVariables& getEnvVars() const;
		const std::string&	getExtension() const;
		const std::string&	getScriptPath() const;
		unsigned int		getTimeoutMs() const;

	protected:
		Request();
		~Request();
		Request(const Request &copy);
		Request &operator=(const Request &assign);

		CgiModule::User				m_user;
		CgiModule::IO_Handler		m_readHandler;
		CgiModule::IO_Handler		m_writeHandler;
		CgiModule::Callback 		m_userCallbacks[CALLBACK_COUNT];

		unsigned int				m_timeoutMs;
		CgiModule::InterpExtension		
									m_extension;
		std::string					m_scriptPath;
		CgiModule::EnvVariables		m_env;

		CgiModule::RequestState		m_state;
		CgiModule::RuntimeOptions	m_options;

		void						mf_reset();
};	


#endif