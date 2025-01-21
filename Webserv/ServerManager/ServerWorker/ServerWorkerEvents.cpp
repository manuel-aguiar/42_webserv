

# include "ServerWorker.hpp"
# include "../../EventCallback/EventCallback.hpp"

void ServerWorker::EventCallbackExit(EventCallback& callback)
{
    ServerWorker* server;

    server = reinterpret_cast<ServerWorker*>(callback.getData());
    server->stop();
}
