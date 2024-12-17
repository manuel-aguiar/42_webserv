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
class Connection;


class CgiRequestData
{
	public:
		CgiRequestData();
		~CgiRequestData();
		CgiRequestData(const CgiRequestData &copy);
		CgiRequestData &operator=(const CgiRequestData &copy);

		Event&				getEventHandler(const e_CgiEvents eventType);
		void				setEventHandler(const e_CgiEvents eventType, const t_ptr_event_data data, const t_func_event_handler handler);
		
		const std::string&								getMsgBody() const;
		const std::map<e_CgiEnv, t_CgiEnvValue>& 		getEnvBase() const;
		const std::map<t_CgiEnvKey, t_CgiEnvValue>&		getEnvExtra() const;

		const std::string&								getExtension() const;


		std::string&		accessMsgBody();

		void				setEnvBase(const e_CgiEnv env, const std::string& value);
		void				setEnvExtra(const std::string& key, const std::string& value);
		
	private:
		Event 			m_events[CGI_EVENT_COUNT];
		std::string		m_msgBody;

		std::string		m_extension;


		std::map<e_CgiEnv, t_CgiEnvValue>			m_envBase;
		std::map<t_CgiEnvKey, t_CgiEnvValue>		m_envExtra;

};	


#endif