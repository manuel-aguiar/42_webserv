/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPool.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:28:10 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/12 10:35:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP

# define CGI_HANDLER_HPP

# include "../Webserver_Definitions.h"
# include "../Event/Event.hpp"

# include <queue>
# include <list>

# define MAX_CONCURRENT_CGI 10

class Event;

class CgiRequest;

class CgiManager
{
    public:
        CgiManager(const size_t maxConcurrent = 1);
        ~CgiManager();

    private:

		HeapArray<CgiRequest>	m_LiveRequests;
        size_t              	m_maxConcurrent;
        size_t              	m_currentConcurrent;
};


#endif
