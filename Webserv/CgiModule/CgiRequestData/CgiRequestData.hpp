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


// forward declarations
class Connection;


class CgiRequestData
{
	public:
		CgiRequestData();
		~CgiRequestData();
		CgiRequestData(const CgiRequestData &copy);
		CgiRequestData &operator=(const CgiRequestData &copy);

		Event&				getEventHandler(const t_CgiEvents eventType);
		void				setEventHandler(const t_CgiEvents eventType, const t_ptr_event_data data, const t_func_event_handler handler);
		
		const std::string&	getMsgBody() const;	
		std::string&		accessMsgBody();
		
	private:
		Event 			m_events[CGI_EVENT_COUNT];
		std::string		m_msgBody;

		//environment variables
		std::string	m_request;
		std::string	m_response;
		std::string	m_status;
		std::string	m_path;
		std::string	m_method;
		std::string	m_body;
		std::string	m_query;
		std::string	m_contentType;
		std::string	m_contentLength;
		std::string	m_host;
		std::string	m_port;
		std::string	m_serverName;
		std::string	m_serverAddr;
		std::string	m_serverPort;
		std::string	m_serverProtocol;
		std::string	m_serverSoftware;
		std::string	m_gatewayInterface;
		std::string	m_remoteAddr;
		std::string	m_remotePort;
		std::string	m_remoteUser;
		std::string	m_remoteIdent;
		std::string	m_authType;
		std::string	m_requestMethod;
		std::string	m_pathInfo;
		std::string	m_pathTranslated;
		std::string	m_scriptName;
		std::string	m_queryString;
		std::string	m_remoteHost;
		std::string	m_remoteDomain;
		std::string	m_redirectRequest;
		std::string	m_redirectURL;
		std::string	m_redirectStatus;
		std::string	m_redirectStatusText;
		std::string	m_redirectHandler;
		std::string	m_redirectQuery;
		std::string	m_redirectURI;
		std::string	m_redirectDocumentRoot;
		std::string	m_redirectScriptFilename;
		std::string	m_redirectScriptName;
		std::string	m_redirectScriptURI;
		std::string	m_redirectScriptURL;
		std::string	m_redirectGatewayInterface;
		std::string	m_redirectServerName;
		std::string	m_redirectServerPort;
		std::string	m_redirectServerProtocol;
		std::string	m_redirectServerSoftware;
		std::string	m_redirectRemoteAddr;
		std::string	m_redirectRemotePort;
		std::string	m_redirectRemoteUser;
		std::string	m_redirectRemoteIdent;
		std::string	m_redirectAuthType;
		std::string	m_redirectRequestMethod;
		std::string	m_redirectPathInfo;
		std::string	m_redirectPathTranslated;
		std::string	m_redirectScriptName;
		std::string	m_redirectQueryString;
		std::string	m_redirectRemoteHost;
		std::string	m_redirectRemoteDomain;
		std::string	m_documentRoot;
		std::string	m_serverAdmin;
		std::string	m_serverSignature;
};	


#endif