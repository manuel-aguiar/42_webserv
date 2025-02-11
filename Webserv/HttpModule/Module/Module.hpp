

#ifndef HTTPMODULE_HPP

# define HTTPMODULE_HPP

// project headers
# include "../HttpDefinitions.hpp"
# include "../../TimerTracker/TimerTracker.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/StackArray/StackArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"


// forward declarations
class DefaultConfig;
class Timer;
class Globals;
namespace Conn {class Connection;}
namespace Http {class Connection;}

namespace Http
{
	class Module
	{
		public:
			Module(const size_t maxConnections, DefaultConfig& config, Globals& globals);
			~Module();

			static void    	InitConnection(Conn::Connection& connection);
			static void		ForcedClose(Conn::Connection& connection);
		
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