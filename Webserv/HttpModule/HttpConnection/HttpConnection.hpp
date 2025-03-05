

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

			struct Timeout
			{
				typedef enum
				{
					NONE,
					KEEP_ALIVE,
					FULL_HEADER,
					INTER_SEND,
					INTER_RECV
				}	Type;
			};

			static void ReadWrite_Callback(Events::Subscription& subscription);

			void 	ReadWrite();

			const Conn::Connection* 	getTCP() const; // sockaddr, fd, ServerContext
			const Ws::Sock::addr& 		getClientAddr() const; // sockaddr -> BlockFinder
			ServerContext& 				accessServerContext(); //Conn::Connection

			void 						setMyTimer(TimerTracker<Timer, Http::Connection*>::iterator timer, const Http::Connection::Timeout::Type timeoutType);

			void 						prepareConnection(Conn::Connection& tcpConn);

			// clean all stored state to be reused later
			void						closeConnection();
			void						closeTransaction();	
			void						resetTransaction();



		private:
		
			Http::Module& 										m_module;
			Timeout::Type 										m_liveTimeout;
			TimerTracker<Timer, Http::Connection*>::iterator 	m_myTimer;
			Conn::Connection* 									m_tcpConn;
			Http::Transaction 									m_transaction;

			// buffers
			Buffer<1024>										m_readBuffer;
			Buffer<1024>										m_writeBuffer;


			Http::IOStatus::Type								mf_readEventHandler();
			Http::IOStatus::Type 								mf_parseReadBuffer();
			Http::IOStatus::Type								mf_newRequestSameConnection();
	};
}

#endif
