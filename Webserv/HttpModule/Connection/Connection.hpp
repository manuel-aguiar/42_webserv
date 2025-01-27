

#ifndef HTTPCONNECTION_HPP

# define HTTPCONNECTION_HPP

namespace Events { class Subscription; }
namespace Conn { class Connection; }

namespace Http
{
	class Connection
	{
		public:
			Connection();
			~Connection();
			Connection(const Connection& other);
			Connection& operator=(const Connection& other);

			static void IO_Callback(Events::Subscription& subscription);
			void onIO();
			void setMyTCP(Conn::Connection* tcpConn);
		
		private:
			Conn::Connection* m_tcpConn;
	};
};

#endif