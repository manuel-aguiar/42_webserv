

#ifndef HTTPMODULE_HPP

# define HTTPMODULE_HPP

// project headers
# include "../HttpDefinitions.hpp"
# include "../../TimerTracker/TimerTracker.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/StackArray/StackArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"


// forward declarations
struct DefaultConfig;
class Timer;
class Globals;
namespace Conn {class Connection;}
namespace Http {class Connection;}

namespace Http
{
	class Module
	{
		public:
			Module(const size_t maxConnections, const DefaultConfig& config, Globals& globals);
			~Module();

			//starter pack
			static void    	InitConnection(Conn::Connection& connection);
			static void		ForcedClose(Conn::Connection& connection);

			// event loop
			int 				closeTimedOutConnections();

			// connection management
			Http::Connection*	acquireConnection();
			void                returnConnection(Http::Connection& connection);

			// timer management
			TimerTracker<Timer, Http::Connection*>::iterator 
			insertTimer(const Timer& timer, Http::Connection& connection);

			void
			removeTimer(TimerTracker<Timer, Http::Connection*>::iterator position);

			// config getters
			int				getClientMaxHeaderSize() const;
			int				getClientMaxBodySize() const;
			int				getTimeoutValue(const Http::Timeout::Type type) const;

		private:

			// infrastructure
			Globals&								m_globals;
			int										m_global_maxHeaderSize;
			StackArray<int, Http::Timeout::COUNT>	m_global_timeouts;
			HeapArray<Http::Connection>				m_connections;
			HeapCircularQueue<Http::Connection*>	m_availableConnections;
			TimerTracker<Timer, Http::Connection*>	m_timers;


	};
};

#endif