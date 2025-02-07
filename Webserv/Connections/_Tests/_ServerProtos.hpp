

#ifndef _SERVERPROTOS_HPP

# define _SERVERPROTOS_HPP

# include "../../Ws_Namespace.h"

namespace Conn { class Connection; }
namespace Events { class Subscription; }

/*
	Protocol that writes the byte 200 to client and immediately closes the connection
	Client reads the byte, confirms it is 200, and if so, success.
	Features only the function InitConnection, all is done there.

	ServeCount will count eh ammount of connections accepted and sent data to.


	Doesn't need a "Request" object, all is done in the InitConnection function.
*/
struct Server_FastCloseModule
{
	Server_FastCloseModule(const Ws::AppLayer::Type type);
	~Server_FastCloseModule();

	static void InitConnection(Conn::Connection& conn);
	int serveCount;
	Ws::AppLayer::Type appLayer;
};

/*
	Protocol that writes the byte 200 to client and never closes the connection.
	Adds and assigns the ForcedClose function to the connection's close callback.
	The objective is to wait, either: 
		client closes on timeout; or
		server shuts down;
	Used to test the ForcedClose feature, from the Conn::Manager to the AppLayer protocol.accept4
	serveCount will actually count the ammount of Connectiosn that were force closed.

	Doesn't need a "Request" object, all is done in the InitConnection function.
*/
struct Server_NeverCloseModule
{
	Server_NeverCloseModule(const Ws::AppLayer::Type type);
	~Server_NeverCloseModule();

	static void InitConnection(Conn::Connection& conn);
	static void ForcedClose(Conn::Connection& conn);
	
	int serveCount;
	Ws::AppLayer::Type appLayer;
};

/*
	A closer recreation of what the server should do.
	Server waits for the Client to right, a single byte. 
	Protocol takes the byte, adds 3, and modules by 256.

	So the client writes, waits for the server to read, waits the response.
	And confirms the server sent back the correct response.

	ServerCount will count the ammount of connections that were served.


	Needs a "Request" object to keep track of state (below)
*/

struct Server_MathModule
{
	Server_MathModule(const Ws::AppLayer::Type type);
	~Server_MathModule();

	static void InitConnection(Conn::Connection& conn);
	static void ForcedClose(Conn::Connection& conn);

	int 				serveCount;
	Ws::AppLayer::Type 	appLayer;
};

/*
	Request object for the above protocol.
*/
struct Server_MathRequest
{
	Server_MathRequest(Server_MathModule& module, Conn::Connection& conn);
	~Server_MathRequest();

	static void ReadWrite_Callback(Events::Subscription& conn);

	void ReadWrite();

	unsigned char		received;
	Conn::Connection& 	conn;
	Server_MathModule& 	module;
};


#endif