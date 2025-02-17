

#ifndef HTTPCONNECTION_HPP

# define HTTPCONNECTION_HPP

// Project headers
# include "../../TimerTracker/TimerTracker.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"
# include "../../GenericUtils/Buffer/Buffer.hpp"

// C++ headers
# include <deque>

// forward declarations
namespace	Events { class Subscription; }
namespace	Conn { class Connection; }
namespace	Http { class Module; class Request; class Response;}

namespace Http
{
	struct Transaction
	{
		// moved to a cpp file
		Transaction(Http::Connection& connection, ServerContext& context) : 
			request(connection), 
			response(request, context) {}

		Http::Request	request;
		Http::Response	response;
	};
}

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
			void 						setMyTCP(Conn::HttpConnection& tcpConn);

			Buffer&						accessReadBuffer();
			Buffer&						accessWriteBuffer();

			// clean all stored state to be reused later
			void						close();

		private:
        	Buffer 												m_readBuffer;
        	Buffer 												m_writeBuffer;

			Http::Module& 										m_module;
			Timer 												m_readTimer;
			Timer 												m_writeTimer;
			TimerTracker<Timer, Http::Connection*>::iterator 	m_myTimer;
			Conn::Connection* 									m_tcpConn;
			std::deque<Http::Transaction> 							m_transactions;
	};
};

#endif