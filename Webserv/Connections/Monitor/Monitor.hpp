

#ifndef CONNECTIONMONITOR_HPP

# define CONNECTIONMONITOR_HPP

namespace Events { class Subscription; }
namespace Events { class Manager; }

class Monitor
{
	public:
		Monitor(Events::Manager& eventManager);
		~Monitor();
		Monitor(const Monitor& copy);
		Monitor& operator=(const Monitor& assign);

		void						reset(bool isCalledFromEventLoop);
		void						acquire();
		void						release();
		void						subscribe(bool isCalledFromEventLoop);
		void						unsubscribe(bool isCalledFromEventLoop);
		void						modify(bool isCalledFromEventLoop);
		Events::Subscription&		accessEvent();
		const Events::Subscription&	getEvent() const;

		Events::Manager&			accessEventManager();
		const Events::Manager&		getEventManager() const;

	private:
		Events::Subscription*		m_eventSubs;
		Events::Manager&			m_eventManager;


};

#endif
