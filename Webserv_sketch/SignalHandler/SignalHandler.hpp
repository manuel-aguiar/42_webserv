/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:02:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/02 09:00:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALHANDLER_HPP

# define SIGNALHANDLER_HPP

# include <signal.h>
# include <vector>
# include <unistd.h>


class WebServerSignalHandler
{
	public:

		static int PipeRead(int serverID) {return _g_pipes[serverID].first;}
		static int PipeWrite(int serverID) {return _g_pipes[serverID].second;}

		static int getSignal() {return (WebServerSignalHandler::_g_signal);};

		static void		signal_handler(int sigNum)
		{
			if (sigNum == SIGINT || sigNum == SIGQUIT)
			{
				_g_signal = sigNum;
				for (size_t i = 0; i < _g_pipes.size(); ++i)
					write(PipeWrite(i), "DUKE NUKEM", sizeof("DUKE NUKEM"));
			}
		}

		static int prepare_signal(struct sigaction *ms, void (*handler)(int), int numServers)
		{
			int pipefd[2];

			ms->sa_flags = 0;
			ms->sa_handler = handler;
			sigemptyset(&(ms->sa_mask));    //unprotected
			sigaction(SIGINT, ms, NULL);    //unprotected
			sigaction(SIGQUIT, ms, NULL);    //unprotected
			for (int i = 0; i < numServers; ++i)
			{
				pipe(pipefd);
				_g_pipes.push_back(std::make_pair(pipefd[0], pipefd[1]));
			}
			return (1);
		}
		static void destroy_signal(struct sigaction *ms)
		{
			(void)ms;

			for (size_t i = 00; i < _g_pipes.size(); ++i)
			{
				close(PipeRead(i));
				close(PipeWrite(i));
			}
			_g_pipes.clear();
		}
	private:
		static	std::vector<std::pair<int, int> >	_g_pipes;
		static int 						            _g_signal;
};

int		WebServerSignalHandler::_g_signal = 0;
std::vector<std::pair<int, int> > WebServerSignalHandler::_g_pipes;

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
	WebServerSignalHandler::prepare_signal(&signal, WebServerSignalHandler::signal_handler, numServers);

*/


#endif