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
# include "../../Event/Event.hpp"

// C++ headers
# include <string>
# include <map>

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

		// methods
		void											reset();

		// getters
		const std::string&								getMsgBody() const;
		const t_CgiRequestEnv& 							getEnvVars() const;
		const std::string&								getExtension() const;
		const std::string&								getScriptPath() const;
			
		t_fd											getReadFd() const;
		t_fd											getWriteFd() const;

		// accessors
		Callback&										accessCallback(const e_CgiCallback eventType);
		EventManager*									accessEventManager();
		
		// setters
		void											setCallback(const e_CgiCallback eventType, 
																		const t_ptr_callback_data data, 
																		const t_func_callback_handler handler);

		void											setMsgBody(const std::string& body);
		void											setEnvBase(const e_CgiEnv env, const std::string& value);
		void											setEnvExtra(const std::string& key, const std::string& value);
		void											setExtension(const std::string& extension);
		void											setScriptPath(const std::string& path);
		void											setEventManager(EventManager& manager);

	protected:
		t_fd											m_readFd;
		t_fd											m_writeFd;
		
	private:
		Callback 										m_callbacks[E_CGI_CALLBACK_COUNT];
		std::string										m_extension;
		std::string										m_scriptPath;
		t_CgiRequestEnv									m_env;
		EventManager*									m_eventManager;

};	


#endif