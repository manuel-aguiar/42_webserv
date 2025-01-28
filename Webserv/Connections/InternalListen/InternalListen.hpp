

#ifndef INTERNALLISTEN_HPP

# define INTERNALLISTEN_HPP

# include "../ListeningSocket/ListeningSocket.hpp"

class ImplManager;
namespace Events {class Subscription;}

class InternalListen : public ListeningSocket
{
	public:
		InternalListen(const int backlog, const Ws::BindInfo& info, Globals& globals, ImplManager& connManager);
		~InternalListen();

		static void                 EventCallbackAccept(Events::Subscription& callback);

		void						setEvent(Events::Subscription* event);
		Events::Subscription*		accessEvent();

	private:
		Events::Subscription*		m_event;
};



#endif