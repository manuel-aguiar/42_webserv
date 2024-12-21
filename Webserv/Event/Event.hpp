/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/21 12:50:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

# include "../GenericUtils/Webserver_Definitions.h"
# include "../Callback/Callback.hpp"



class Event
{
	public:
		Event();
		~Event();
		Event(const Event& copy);
		Event& operator=(const Event& assign);

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
		void						setMonitorFlags(int flags);
		void						setTriggeredFlags(int flags);

		void						setFdFlags(const t_fd fd, int flags);
		void						setCallback(const t_ptr_callback_data data, const t_func_callback_handler handler);
		void						setFdFlagsCallback(const t_fd fd, int flags, const t_ptr_callback_data data, const t_func_callback_handler handler);


		// accessors
		Callback&					accessCallback();

	private:
		t_fd						m_fd;
		int							m_monitoredFlags;
		int							m_triggeredFlags;
		Callback					m_callback;

};

#endif
