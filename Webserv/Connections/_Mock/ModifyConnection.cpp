

# include "ModifyConnection.hpp"
# include "../../Events/Subscription/Subscription.hpp"




void
ModifyConnection::setConnInfo(const ConnInfo& info)
{
	m_info = info;
}

ModifyConnection::ModifyConnection(Events::Manager& eventManager, ServerContext& context) :
	Conn::Connection(eventManager, context, NULL)
{
}

ModifyConnection::~ModifyConnection()
{
}

ModifyConnection::ModifyConnection(const ModifyConnection& copy) :
	Conn::Connection(copy) {}

ModifyConnection&
ModifyConnection::operator=(const ModifyConnection& assign)
{
	if (this == &assign)
		return (*this);
	Conn::Connection::operator=(assign);
	return (*this);
}

