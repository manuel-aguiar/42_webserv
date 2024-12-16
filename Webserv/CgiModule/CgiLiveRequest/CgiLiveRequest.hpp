/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:42:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:57:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

//Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Event/Event.hpp"

class EventManager;
class CgiModule;
class CgiRequestData;
class Globals;
class Connection;

class CgiLiveRequest
{
	public:
		CgiLiveRequest(CgiModule& manager, Globals& globals);
		~CgiLiveRequest();

		void    execute();
		void    reset();
		void    prepare(EventManager& manager, const CgiRequestData& request, Connection& connection);

	private:
		// pointers, will be reused
		EventManager*				m_eventManager;
		CgiRequestData*				m_pendingRequest;
		Connection*					m_connection;
		
		Event						m_readEvent;
		Event						m_writeEvent;

		t_fd						m_ParentToChild[2];
		t_fd						m_ChildToParent[2];
		t_pid						m_pid;

		CgiModule&					m_manager;
		Globals&					m_globals;


		// helpers
		void						closeFd(t_fd& fd);


		CgiLiveRequest(const CgiLiveRequest &other);
		CgiLiveRequest &operator=(const CgiLiveRequest &other);
};



#endif
