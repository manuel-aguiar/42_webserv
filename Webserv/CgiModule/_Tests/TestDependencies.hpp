/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestDependencies.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:22:07 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 14:45:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTDEPENDENCIES_HPP

# define TESTDEPENDENCIES_HPP

#include "../Cgi_Definitions.h"

class Event;

class CgiUser
{

};

class CgiUserGateway
{
	public:
		// Generic handlers to provide to CgiRequestData
		static void onRead(Event& event);
		static void onWrite(Event& event);
		static void onError(Event& event);
		static void onClose(Event& event);
		static void onTimeout(Event& event);

		static void (*eventHandlers[E_CGI_EVENT_COUNT])(Event& event);

		// Implementation of events
		static void CgiOnRead(CgiUser& request);
		static void CgiOnWrite(CgiUser& request);
		static void CgiOnError(CgiUser& request);
		static void CgiOnClose(CgiUser& request);
		static void CgiOnTimeout(CgiUser& request);	
};


#endif
