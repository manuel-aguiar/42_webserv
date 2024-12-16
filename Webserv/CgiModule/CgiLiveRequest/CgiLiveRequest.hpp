/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:42:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 16:18:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQUEST_HPP

# define CGIREQUEST_HPP

//Project Headers
# include "../CgiDefinitions.h"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Event/Event.hpp"

class EventManager;
class CgiModule;
class CgiRequestData;
class CgiPendingRequest;
class Globals;
class Connection;

// implement some timeout for script execution, if nothing then kill

class CgiLiveRequest
{
	public:
		CgiLiveRequest(CgiModule& manager, Globals& globals);
		~CgiLiveRequest();

		void    execute();
		void    reset();
		void    prepare(EventManager& manager, const CgiPendingRequest& request, Connection& connection);
		
		void	forcedClose();

		void	writeToChild();
		void	readFromChild();



		EventManager*				accessCurEventManager() const;	

	private:
		// pointers, will be reused
		EventManager*				m_curEventManager;
		CgiRequestData*				m_curRequestData;
		
		char**						m_argv;
		char**						m_envp;
		
		Event						m_readEvent;
		Event						m_writeEvent;

		t_fd						m_ParentToChild[2];
		t_fd						m_ChildToParent[2];
		t_pid						m_pid;

		CgiModule&					m_manager;
		Globals&					m_globals;

		char						m_readBuf[1024];

		// helpers
		void						mf_closeFd(t_fd& fd);
		void						mf_executeParent();
		void						mf_executeChild();


		// internal events
		static void					mf_CgiWrite(Event& event);
		static void					mf_CgiRead(Event& event);



		CgiLiveRequest(const CgiLiveRequest &other);
		CgiLiveRequest &operator=(const CgiLiveRequest &other);
};



#endif
