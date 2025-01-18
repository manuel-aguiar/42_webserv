/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiRequestData.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:01:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:52:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQUESTDATA_HPP

# define CGIREQUESTDATA_HPP

//Project Headers
# include "../Cgi_Definitions.h"
# include "../Test_Event/Test_Event.hpp"

// C++ headers
# include <string>

// forward declarations
class EventManager;

class CgiRequestData
{
	protected:
		CgiRequestData();
		~CgiRequestData();
		CgiRequestData(const CgiRequestData &copy);
		CgiRequestData &operator=(const CgiRequestData &assign);

	public:

		typedef int		t_bytesRead;
		typedef int		t_bytesWritten;
		typedef int		t_bytesReadWritten;
		typedef void*	t_ptr_CgiUser;
		typedef void 	(*t_func_CgiHandler)(t_ptr_CgiUser user);
		typedef t_bytesReadWritten 	(*t_cgi_IO)(t_ptr_CgiUser user, int targetFd);

		// methods
		void											reset();

		const std::string&								getMsgBody() const;
		const t_CgiRequestEnv& 							getEnvVars() const;
		const std::string&								getExtension() const;
		const std::string&								getScriptPath() const;
		unsigned int									getTimeoutMs() const;


		// setters
		void											setUser(t_ptr_callback_data user);

		void											setHandler(const e_CgiCallback type, 
																		const t_func_CgiHandler handler);

		void											setReadHandler(t_cgi_IO handler);
		void											setWriteHandler(t_cgi_IO handler);

		void											setEnvBase(const e_CgiEnv env, const std::string& value);
		void											setEnvExtra(const std::string& key, const std::string& value);
		void											setExtension(const std::string& extension);
		void											setScriptPath(const std::string& path);
		void											setTimeoutMs(const unsigned int timeoutMs);

	protected:

		// user data
		t_ptr_CgiUser									m_user;
		t_func_CgiHandler 								m_stateCallbacks[E_CGI_CALLBACK_COUNT];
		t_cgi_IO										m_readHandler;
		t_cgi_IO										m_writeHandler;


		// actual data
		std::string										m_extension;
		std::string										m_scriptPath;
		unsigned int									m_timeoutMs;
		t_CgiRequestEnv									m_env;

};	


#endif