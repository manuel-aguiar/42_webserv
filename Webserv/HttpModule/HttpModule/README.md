

#### Module Notes:

The Http::Module will hold any state that is needed by all connections.
Case in point:
	- Http max header len (config)
	- Http max body size (config)
	- Http allowed methods (config)
	- Timers, 1 per connection.



#### Timers

There are 2 types of timers: Connection level timers and Request level timers

Connection level -> track innactivity of a connection, to close it.
Request level -> track the MAXIMUM time to receive a full request since the first byte is received
	-> this timeout is very important because an attacker may send bytes slowly 1 by 1
	-> we want to close the request and therefore the connection if the parser doesn't indicate that the 
	request has COMPLETE info to formulate a response

Connection timeout must be BIGGER than request timeout.
Also, the timing out of a request should be bigger than the timeout of the Connection.
Therefore, the limiting factor is the request timeout.
Also, a request timeout implies a connection timeout as a safety measure. This is helpful
as we will see below, since we have a maximum number of connections by config, we will
have effectively a max number of timers associated to Http::Connection (capped by Conn::Connection)
So, the Http::Module will have a TimerTracker<Timer, Http::Connection*>, with size maxConnections (ServerConfig)
It will be up to the Http::Connection keep the relevant timer in place, either a Connection timer or a 
Request timer.

Additionally, absense of activity by a connection must be computed as the absense of read AND write events.
Imagine the case where we want to write a response to the client socket but the client is not draining
	-> no write events, we are stuck waiting, time the connection out.

Timeline:

1) connection established (accepted), Http::Connection contacts the Http::Module to set the Connection timeout
2) a byte is received (or more). The parser indicates that it is a new request. The Http::Connection
	will update its timer with Http::Module -> unsubscribe Connection timer and subscribe the Request timer
	WEHILE STILL POINTING to the same Http::Connection (request timeout implies connection timeout)
3) more bytes are received. Parser indicates that the request is fulfilled. 
	If the request has more bytes than \r\n\r\n then:
		new request -> update the timer to request-timeout (config)
	else
		no more requests in the pipeline, update the timer to the connection-timeout

4) Event Loop: each turn, the Http::Module will close timedout connections, (either because of request, or connection)
	Http::Connection will close all associated state (requests, pending responses) and close the client connection

5) same as the CgiModule, the TimerTracker will provide the shortest time until the next timeout, that extra time
	will be put on epoll_wait, effectively capping the waiting time of epoll
	-> if no events are received in the meantime, the Http::Module will be called to close timedout connections

### Structure:

Http::Module -> holds state, config default values, allowed methods
			-> establishes the InitConnection function
			-> established the ForcedClose function
			-> since the number of max connections is defined at config, we can
			preallocate the number of Http::Connections as a HeapArray, and have a HeapCircularQueue
			to acquire and retrieve Http::Connection objects as they come and go
			-> holds the timer tracker for those httpConnections

Http::Connection -> bridge between the Http::Module and Conn::Connection
				-> responsible for reading and writing, event monitoring
				-> holds a queue of requests
				-> holds a queue of responses
				-> holds a pointer to module to access config values
				-> potentially a pointer to ServerContext to access addons, but for quicker dereferencing, the pointer to module should
				still be there
				-> holds a pointer to its Timer in the Http::Module::TimerTracker for O(1) lookup (+ Ologn delete + Ologn re-insert)
					(at least we don't iterate the multimap to find our own timer)

Http::Request -> holds the request data parameters: concrete headers, path, cgi requirements, etc
				-> since keep-alive, the number of queued requests may not be none at the beginning of runtime, it is effectively limitless
					and so, it cannot be preallcoated

Http::Response -> generates the response buffer to be sent by the Http::Connection,
				-> queue of Responses, unknown size, same as Http::Request


### Pseudo headers:

Overall, the Http::Module will hold all connections which in turn will hold their respective Requests and Responses

namespace Http
{
	class Module
	{
		public:
			Module (ServerConfig& config)

			// event loop method, close timedout, returns time until next timeout
			int	closeTimedOutConnections();

			// Connection utilities
			Http::Connection* 	acquireConnection();
			void				returnConnection(Http::Connection& connection);
			void				insertTimer(Timer& timer, Http::Connection& user);
			void				modifyTimer(Http::Connection& user);

		private:
			HeapArray<Http::Connection>				m_connections;
			HeapCircularQueue<Http::Connection*>	m_availableConnections;
			TimerTracker<Timer, Http::Connection*>	m_timers
	}
}

namespace Http
{
	class Connection
	{
		public:
			Connection(Http::Module& module, ServerContext& context);

			static void ReadWrite_Callback(Events::Subscription& subs);  // event callback for the Events::Manager
			
			
			// entry point for creating a request, determine read write, call the Module to establish timers
			void	ReadWrite();											// called from the callback, after typecasting the subs.user

			//
			void	close();			// called by bad_request etc, called by Http::Module::closeTimedOutConnections()

		private:
			char 						m_readBuffer[1024];
			char						m_writeBuffer[1024];		//maybe

			Http::Module&										m_module;
			ServerContext&										m_context;			// access to the CgiModule, for instance, BlockFinder, etc
			TimerTracker<Timer, Http::Connection*>::iterator	m_myTimer;
			Conn::Connection*									m_myTCP;			// a pointer to the Conn::Connection, access to events, fd, sockaddr....
																					// pointer because Http::Connection object is recycled by Http::Module
			std::deque<Http::Request>							m_requests;
			std::deque<Http::Response>							m_responses;
	}
}

namespace Http
{
	class Request
	{
		public:
			Request(Http::Connection& conn);		//keep a reference to its "owner"



		private:
			// variables for the request
			[...]
			Http::Connection&			m_myconnection;
	}
}

namespace Http
{
	class Response
	{
		public:
			Response(Http::Connection& myConnection, Http::Request& myRequest, ServerContext& context);



		private:
			Http::Connection&	m_myConnection;			// tell Connection to write response
			Http::Request&		m_myRequest;			// access data from Request to prepare response
			ServerContext&		m_context;				// access infrastructure (CgiModule, BlockFinder)

			[...]

			//Some more variables here to access the interface of the CgiModule and save state in between event loop calls

	}
}