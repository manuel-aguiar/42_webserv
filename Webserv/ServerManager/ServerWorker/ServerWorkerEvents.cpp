

# include "ServerWorker.hpp"
# include "../../Event/Event.hpp"

void ServerWorker::EventCallbackExit(Callback& callback)
{
    ServerWorker* server;

    server = reinterpret_cast<ServerWorker*>(callback.getData());
    server->stop();
}
