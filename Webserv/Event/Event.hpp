

#ifndef EVENT_HPP

# define EVENT_HPP

# include "../GenericUtils/Webserver_Definitions.h"
# include "../Callback/Callback.hpp"

class Event
{
	public:
		Event();
		~Event();

		//methods
		void						handle();
		void						reset();

		//getters
		t_fd						getFd()					const;
		int							getMonitoredFlags()		const;
		int							getTriggeredFlags()		const;
		const Callback&				getCallback()			const;
		
		//setters
		void						setFd(const t_fd fd);
		void						setMonitoredFlags(int flags);
		void						setTriggeredFlags(int flags);
		void						setCallback(const t_ptr_callback_data data, const t_func_callback_handler handler);


		void						setFdFlags(const t_fd fd, int flags);
		void						setFdFlagsCallback(const t_fd fd, int flags, const t_ptr_callback_data data, const t_func_callback_handler handler);


	protected:
		t_fd						m_fd;
		int							m_monitoredFlags;
		int							m_triggeredFlags;
		Callback					m_callback;
		
		Event(const Event& copy);
		Event& operator=(const Event& assign);


		//for internal use
		t_fd						m_subscribedFd;
		int							m_subscribedFlags;
};

#endif
