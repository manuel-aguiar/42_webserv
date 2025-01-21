

# include "ListeningSocket.hpp"
# include "../../EventCallback/EventCallback.hpp"

void ListeningSocket::EventCallbackAccept(EventCallback& callback)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(callback.getData());
	listener->accept();
}
