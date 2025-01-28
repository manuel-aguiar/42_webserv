

#ifndef CONNECTIONMONITOR_HPP

# define CONNECTIONMONITOR_HPP

namespace Events { class Subscription; }
namespace Events { class Manager; }

class Monitor
{
	public:
		Monitor(Events::Manager& eventManager);
		~Monitor();

		void						reset(bool isCalledFromEventLoop);
		void						acquire();
		void						release();
		void						subscribe(bool isCalledFromEventLoop);
		void						unsubscribe(bool isCalledFromEventLoop);
		void						modify(bool isCalledFromEventLoop);
		Events::Subscription*		accessEvent();

	private:

		typedef enum
		{
			SUBSCRIBED,
			UNSUBSCRIBED
		}	State;

		Events::Subscription*		m_eventSubs;
		Events::Manager&			m_eventManager;

		State						m_state;

		Monitor(const Monitor& copy);
		Monitor& operator=(const Monitor& assign);

};

#endif