

#ifndef CONNECTIONMONITOR_HPP

# define CONNECTIONMONITOR_HPP

namespace Events { class Subscription; }
namespace Events { class Manager; }

class Monitor
{
	public:
		Monitor();
		~Monitor();

		void						reset(Events::Manager& eventManager, bool isCalledFromEventLoop);
		void						acquire(Events::Manager& eventManager);
		void						release(Events::Manager& eventManager);
		void						subscribe(Events::Manager& eventManager, bool isCalledFromEventLoop);
		void						unsubscribe(Events::Manager& eventManager, bool isCalledFromEventLoop);
		void						modify(Events::Manager& eventManager, bool isCalledFromEventLoop);
		Events::Subscription&		accessEvent();
		const Events::Subscription&	getEvent() const;

	private:
		Events::Subscription*		m_eventSubs;

		Monitor(const Monitor& copy);
		Monitor& operator=(const Monitor& assign);

};

#endif