

# include "Connection.hpp"
# include "../../Events/Subscription/Subscription.hpp"

namespace Http
{
	void Connection::IO_Callback(Events::Subscription& subscription)
	{
		Http::Connection* httpConn = reinterpret_cast<Http::Connection*>(subscription.accessUser());
		httpConn->onIO();
	}
}