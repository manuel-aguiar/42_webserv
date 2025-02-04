

# include "AppLayer.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

# include <unistd.h>

AppLayer::AppLayer() : 
	m_appConn(NULL),
	m_appForceClose(NULL) {}

AppLayer::~AppLayer() {}

AppLayer::AppLayer(const AppLayer& copy) : 
	m_appConn(copy.m_appConn),
	m_appForceClose(copy.m_appForceClose) {}

AppLayer&
AppLayer::operator=(const AppLayer& assign)
{
	if (this == &assign)
		return (*this);

	m_appConn = assign.m_appConn;
	m_appForceClose = assign.m_appForceClose;

	return (*this);
}

void
AppLayer::setConn				(const Ws::AppLayer::Conn& appConn)
{
	m_appConn = appConn;
}

void
AppLayer::setCloseCallback	(const Ws::AppLayer::CloseCallback callback)
{
	m_appForceClose = callback;
}

Ws::AppLayer::Conn
AppLayer::accessConn()
{
	return (m_appConn);
}

Ws::AppLayer::CloseCallback
AppLayer::accessCloseCallback()
{
	return (m_appForceClose);
}

void
AppLayer::close(Socket& fill)
{
	ASSERT_EQUAL(fill.getSockFd() != Ws::FD_NONE, true, "AppLayer::close(), socket already closed");
	::close(fill.getSockFd());
	fill.setSockFd(Ws::FD_NONE);
}

void
AppLayer::reset()
{
	m_appConn = NULL;
	m_appForceClose = NULL;
}