

# include "ConnectionManager.hpp"

Manager::ManagedConnection::ManagedConnection(Globals& globals) :
	Connection(globals)
{
}

Manager::ManagedConnection::~ManagedConnection()
{
}

// private
Manager::ManagedConnection::ManagedConnection(const ManagedConnection& other) : Connection(other)
{
	*this = other;
}

Manager::ManagedConnection& Manager::ManagedConnection::operator=(const ManagedConnection& other)
{
	if (this != &other)
		Connection::operator=(other);
	return (*this);
}