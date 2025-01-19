

# include "ListeningSocket.hpp"
# include "../../Event/Event.hpp"

void ListeningSocket::EventCallbackAccept(Callback& callback)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(callback.getData());
	listener->accept();
}
