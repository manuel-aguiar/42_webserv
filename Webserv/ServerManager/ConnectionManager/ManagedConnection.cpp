

# include "ConnectionManager.hpp"

ConnectionManager::ManagedConnection::ManagedConnection(Globals& globals) :
	Connection(globals)
{
}

ConnectionManager::ManagedConnection::~ManagedConnection()
{
}

// private
ConnectionManager::ManagedConnection::ManagedConnection(const ManagedConnection& other) : Connection(other)
{
	*this = other;
}

ConnectionManager::ManagedConnection& ConnectionManager::ManagedConnection::operator=(const ManagedConnection& other)
{
	if (this != &other)
		Connection::operator=(other);
	return (*this);
}