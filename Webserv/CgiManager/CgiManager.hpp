/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/15 08:21:26 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP

# define CGI_HANDLER_HPP

# include "CgiDefaults.h"
# include "../Webserver_Definitions.h"
# include "../Event/Event.hpp"

# include <queue>
# include <list>

# define MAX_CONCURRENT_CGI 10

class Event;
class CgiLiveRequest;
class CgiPendingRequest;

class CgiManager
{
    public:
        CgiManager(const size_t maxConcurrent = 1);
        ~CgiManager();

		const char* const* getDefaultEnvp() const;



    private:

        size_t              			m_maxConcurrent;
        size_t              			m_currentConcurrent;
		HeapArray<CgiLiveRequest>		m_LiveRequests;
		HeapArray<Event>				m_readEvents;


		static const char*	const		m_defaultEnvp[ENV_CGI_VAR_COUNT];
};


#endif
