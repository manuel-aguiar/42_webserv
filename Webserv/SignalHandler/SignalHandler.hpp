

#ifndef SIGNALHANDLER_HPP

# define SIGNALHANDLER_HPP

// Project headers
# include "../../Toolkit/Arrays/DynArray/DynArray.hpp"

// own forward declaration + extern global instance
class SignalHandler;


// THE USER WILL HAVE TO INSTANTIATE THEMSELVES
// allws a user to have a derived class and instantiate that one
// if it wants to
extern SignalHandler 	g_SignalHandler;


// forward declarations
class Globals;

// not making the full namespace a dependency when i just want the fd typedef
namespace Ws {typedef int fd;}

class SignalHandler
{
	private:

	public:
		SignalHandler();
		~SignalHandler();

		void			openSignalListeners(const size_t numServers, Globals& globals);

		void			registerSignal(const int sigNum, Globals& globals);
		void			ignoreSignal(const int sigNum, Globals& globals);
		void			unregisterSignal(const int sigNum, Globals& globals);

		void			notifySignalReception(int sigNum);

		void			setSignal(int sigNum);
		int				getSignal();

		Ws::fd			getSignalListener(int serverID);
		

	private:

		enum {SIG_NONE = 0};

		struct PipeFds
		{
			Ws::fd	read;
			Ws::fd	write;
		}; // helper struct

		void				mf_clearOpenPipes();
		static void			mf_defaultHandler(int sigNum);

		DynArray<PipeFds>	m_pipes;
		int 				m_signal;

		//shared to all instances, force single instance and assert if more than 1
		static bool			m_isInstantiated;

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