
# include "Connection.hpp"
# include "../../Events/Subscription/Subscription.hpp"
namespace Http
{

void
Connection::ReadWrite_Callback(Events::Subscription& subscription)
{
	Http::Connection* connection = reinterpret_cast<Http::Connection*>(subscription.accessUser());
	connection->ReadWrite();

}

}	// end of http namespace