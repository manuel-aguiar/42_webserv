

#ifndef HTTPCONNECTION_HPP

# define HTTPCONNECTION_HPP

// Project headers

# include "../HttpTransaction/HttpTransaction.hpp"

# include "../../Ws_Namespace.h"
# include "../../TimerTracker/TimerTracker.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"
# include "../../GenericUtils/Buffer/Buffer.hpp"


// forward declarations
class 		ServerContext;
namespace	Conn { class Connection; }
namespace	Events { class Subscription; }
namespace	Http { class Module;}

namespace Http
{
	class Connection
	{
		public:
			Connection(Http::Module& module);
			~Connection();
			Connection(const Connection& other);
			Connection& operator=(const Connection& other);

			static void ReadWrite_Callback(Events::Subscription& subscription);

			void 	ReadWrite();

			const Conn::Connection* 	getTCP() const; // sockaddr, fd, ServerContext
			const Ws::Sock::addr& 		getClientAddr() const; // sockaddr -> BlockFinder
			ServerContext& 				accessServerContext(); //Conn::Connection

			void 						setMyTimer(TimerTracker<Timer, Http::Connection*>::iterator timer);

			void 						setMyTCP(Conn::Connection& tcpConn);

			// clean all stored state to be reused later
			void						closeConnection();

			void						resetTransaction();

		private:
		
			Http::Module& 										m_module;
			Timer 												m_readTimer;
			Timer 												m_writeTimer;
			TimerTracker<Timer, Http::Connection*>::iterator 	m_myTimer;
			Conn::Connection* 									m_tcpConn;
			Http::Transaction 									m_transaction;

			// buffers
			Buffer<1024>										m_readBuffer;
			Buffer<150>											m_writeBuffer;


			int													mf_read(const Ws::fd fd);
			void												mf_write(const Ws::fd fd);
	};
}

#endif
