/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:02:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/12 15:21:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALHANDLER_HPP

# define SIGNALHANDLER_HPP

# include "../../GenericUtils/Webserver_Definitions.h"
# include <signal.h>
# include <vector>
# include <unistd.h>


class Globals;
class SignalHandler;

extern SignalHandler 	g_SignalHandler;
extern size_t			gm_counter;

class SignalHandler
{
	public:
		SignalHandler();
		~SignalHandler();

		void									prepare_signal(void (*handler)(int), size_t numServers, Globals& globals);

		static void								signal_handler(int sigNum);

		// getters
		int										getSignal();
		const std::vector<std::pair<t_fd, t_fd> >&		
												getPipes();



		t_fd									getPipeRead(int serverID);
		t_fd									getPipeWrite(int serverID);
		
		void									setSignal(int sig);

	private:
		Globals*								m_globals;
		std::vector<std::pair<t_fd, t_fd> >		m_pipes;
		int 									m_signal;
		t_sigaction								m_sigact;

		//shared to all instances
		static size_t							gm_counter;


		//private
		SignalHandler(const SignalHandler& copy);
		SignalHandler& operator=(const SignalHandler& assign);
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