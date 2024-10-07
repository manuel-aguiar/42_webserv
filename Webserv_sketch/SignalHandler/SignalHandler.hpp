/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:02:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 09:24:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALHANDLER_HPP

# define SIGNALHANDLER_HPP

# include "../Webserver_Definitions.h"
# include <signal.h>
# include <vector>
# include <unistd.h>


class Globals;

class SignalHandler
{
	public:
		static int 			PipeRead(int serverID);
		static int 			PipeWrite(int serverID);
		static int 			getSignal();
		static void			signal_handler(int sigNum);
		static int 			prepare_signal(t_sigaction *ms, void (*handler)(int), int numServers, Globals* globals);
		static void 		destroy_signal(t_sigaction *ms);

	private:
		static Globals*						        _g_globals;
		static std::vector<std::pair<int, int> >	_g_pipes;
		static int 						            _g_signal;

};


/*

    	sigset_t threadSigSet;

	// Disable SIGINT and SIGQUIT for the threadpool workers
	sigemptyset(&threadSigSet);
	sigaddset(&threadSigSet, SIGINT);
	sigaddset(&threadSigSet, SIGQUIT);

	pthread_sigmask(SIG_BLOCK , &threadSigSet, NULL);		// explicitely block sigint/quit for new threads		UNPROTECTED

	int numServers = 10;
	ThreadPool servers(numServers);

	// Re-enable for the main thread
	pthread_sigmask(SIG_UNBLOCK, &threadSigSet, NULL);		// reestablish the oldmask set by the user		UNPROTECTED

	// Signal Handler for the program
	struct sigaction signal = (struct sigaction){};
	SignalHandler::prepare_signal(&signal, SignalHandler::signal_handler, numServers);

*/


#endif