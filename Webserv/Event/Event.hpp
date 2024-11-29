/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/29 10:58:29 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

class Event
{
	public:
		Event();
		~Event();

		//typedefs
		typedef void	(*HandlerFunction)(Event& event);
		typedef void*	HandlerData;

		//methods
		void				handle();

		//getters
		HandlerData			getData();
		HandlerFunction		getFunction();
		int					getFlags();

		//setters
		void				setHandlerFunction_and_Data(HandlerFunction function, HandlerData data);
		void				setFlags(int flags);




	private:
		HandlerFunction		m_function;
		HandlerData			m_data;
		int					m_flags;

		Event(const Event& copy);
		Event& operator=(const Event& assign);
};

#endif
