

#ifndef APP_LAYER_PROTOS_HPP

# define APP_LAYER_PROTOS_HPP

# include "../../Ws_Namespace.h"

namespace Conn { class Connection; }

struct ProtoFastClose
{
	ProtoFastClose(const Ws::AppLayer::Type type);
	~ProtoFastClose();

	static void InitConnection(Conn::Connection& conn);
	int serveCount;
	Ws::AppLayer::Type appLayer;
};

struct ProtoNeverClose
{
	ProtoNeverClose(const Ws::AppLayer::Type type);
	~ProtoNeverClose();

	static void InitConnection(Conn::Connection& conn);
	static void ForcedClose(Conn::Connection& conn);
	
	int serveCount;
	Ws::AppLayer::Type appLayer;
};

struct ProtoSingleRequest;

struct SingleRequest
{
	SingleRequest(ProtoSingleRequest& module, Conn::Connection& conn);
	~SingleRequest();

	static void ReadWrite_Callback(Events::Subscription& conn);

	void ReadWrite();

	Conn::Connection& 	conn;
	ProtoSingleRequest& module;
};

struct ProtoSingleRequest
{
	ProtoSingleRequest(const Ws::AppLayer::Type type);
	~ProtoSingleRequest();

	static void InitConnection(Conn::Connection& conn);
	static void ForcedClose(Conn::Connection& conn);

	int serveCount;
	Ws::AppLayer::Type appLayer;
};

#endif