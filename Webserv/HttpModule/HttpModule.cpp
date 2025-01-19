

# include "HttpModule.hpp"
# include "../Connection/Connection.hpp"
# include "HttpConnection/HttpConnection.hpp"

HttpModule::HttpModule(ServerManager& serverManager) :
	m_serverManager(serverManager),
	m_httpmanager(m_serverManager)
{

}

HttpModule::~HttpModule()
{

}


/*
	Connection has: pointer to the module.
	The init function:
	- allocates an HttpConnection class inside the Connection memory pool,
	- links Connection to HttpConnection,
	- tells the httpmanager to store that pointer (it may not be needed in the end if httpconnections are truly independent)
		(in which case, removing sessions the httpmanager state is rather useless and we let each Connection manage itself with even handlers....)
*/
void HttpModule::initConnection(Connection* accepted)
{
	HttpManager& 	manager = (reinterpret_cast<HttpModule*>(accepted->accessProtoModule()))->accessHttpManager();

	HttpConnection* conn = (HttpConnection* )accepted->accessMemPool().allocate(sizeof(HttpConnection));
	new (conn) HttpConnection(accepted);

	accepted->setProtoConnection(conn);
	manager.addConnection(conn);
}


