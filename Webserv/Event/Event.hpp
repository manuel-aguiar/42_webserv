/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 17:02:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

class Event
{
	public:
		Event();
		~Event();
		Event(const Event& copy);
		Event& operator=(const Event& assign);



		//methods
		void				handle();

		//getters
		t_fd						getFd()			const;
		const t_ptr_event_data		getData()		const;
		const t_func_event_handler	getHandler()	const;
		int							getFlags()		const;

		//setters
		void						setFd(const t_fd fd);
		void						setData(const t_ptr_event_data data);
		void						setHandler(const t_func_event_handler handler);
		void						setFlags(int flags);
		void						setFd_Data_Handler_Flags(const t_fd fd, const t_ptr_event_data data, const t_func_event_handler handler, int flags);

		// accessors
		t_ptr_event_data			accessData();
		t_func_event_handler		accessHandler();

	private:
		t_fd						m_fd;
		t_ptr_event_data			m_data;
		t_func_event_handler		m_handler;
		int							m_flags;

};

#endif
