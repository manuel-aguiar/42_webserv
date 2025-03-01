

#ifndef HTTPMODULE_HPP

# define HTTPMODULE_HPP

// project headers
# include "../HttpDefinitions.hpp"
# include "../../TimerTracker/TimerTracker.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/StackArray/StackArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

# include "../HttpCgiInterface/HttpCgiInterface.hpp"

// forward declarations

class ServerContext;

class Timer;
class Globals;
namespace Conn {class Connection;}
namespace Http {class Connection;}

namespace Http
{
	class Module
	{
		public:
			Module(const size_t maxConnections, ServerContext& context);
			~Module();

			//starter pack
			static void    	InitConnection(Conn::Connection& connection);
			static void		ForcedClose(Conn::Connection& connection);

			// event loop
			int 				closeTimedOutConnections();

			// connection management
			Http::Connection*	acquireConnection();
			void                returnConnection(Http::Connection& connection);

			ServerContext&		accessServerContext();
			Http::CgiInterface& accessCgiInterface();

			// timer management
			TimerTracker<Timer, Http::Connection*>::iterator 
			insertTimer(const Timer& timer, Http::Connection& connection);

			void
			removeTimer(TimerTracker<Timer, Http::Connection*>::iterator position);

		private:
			// infrastructure
			ServerContext&							m_context;
			Http::CgiInterface						m_cgiInterface;
			HeapArray<Http::Connection>				m_connections;
			HeapCircularQueue<Http::Connection*>	m_availableConnections;
			TimerTracker<Timer, Http::Connection*>	m_timers;
	};
}

#endif
