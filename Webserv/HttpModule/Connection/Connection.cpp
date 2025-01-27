
# include "Connection.hpp"

namespace Http
{
	Connection::Connection() {}
	Connection::~Connection() {}
	Connection::Connection(const Connection& other) {(void)other;}
	Connection& Connection::operator=(const Connection& other) {(void)other; return (*this);}

	void	Connection::onIO()
	{
		// gotta read and write, parse, all the good stuff, create a request, save it
	}

	void	Connection::setMyTCP(Conn::Connection* tcpConn)
	{
		m_tcpConn = tcpConn;
	}
}