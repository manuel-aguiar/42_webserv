

#ifndef SIGNALHANDLER_HPP

# define SIGNALHANDLER_HPP

# include "../../Ws_Namespace.h"
# include <signal.h>
# include <vector>
# include <unistd.h>


class Globals;
class SignalHandler;

extern SignalHandler 	g_SignalHandler;

class SignalHandler
{
	public:
		SignalHandler();
		~SignalHandler();

		void									prepare_signal(void (*handler)(int), size_t numServers, Globals& globals);

		static void								signal_handler(int sigNum);

		// getters
		int										getSignal();
		const std::vector<std::pair<Ws::fd, Ws::fd> >&		
												getPipes();

		Ws::fd									getPipeRead(int serverID);
		Ws::fd									getPipeWrite(int serverID);
		

		//setters
		void									setSignal(int sig);

	private:
		Globals*								m_globals;
		std::vector<std::pair<Ws::fd, Ws::fd> >	m_pipes;
		int 									m_signal;
		struct sigaction						m_sigact;

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

	// Signal Callback for the program
	struct sigaction signal = (struct sigaction){};
	SignalHandler::prepare_signal(&signal, SignalHandler::signal_handler, numServers);

*/


#endif