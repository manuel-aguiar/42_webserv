

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

#endif