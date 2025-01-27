
# include "InternalConn.hpp"

InternalConn::InternalConn(	Events::Manager& eventManager, 
							ImplManager& connManager, 
							ServerContext& serverContext) :
	Conn::Connection(eventManager, connManager, serverContext) {}

InternalConn::~InternalConn() {}

InternalConn::InternalConn(const InternalConn& other) :
	Conn::Connection(other) {}

InternalConn& InternalConn::operator=(const InternalConn& other)
{
	if (this == &other)
		return (*this);
	
	Conn::Connection::operator=(other);
	
	return (*this);
}