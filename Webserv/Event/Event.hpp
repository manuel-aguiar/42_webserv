/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 12:00:13 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

class Event
{
	public:
		Event();
		~Event();
		Event(const Event& copy);
		Event& operator=(const Event& assign);

		//typedefs
		typedef void	(*HandlerFunction)(Event& event);
		typedef void*	HandlerData;

		//methods
		void				handle();

		//getters
		HandlerData			getData()		const;
		HandlerFunction		getFunction()	const;
		int					getFlags()		const;

		//setters
		void				setHandlerFunction_and_Data(HandlerFunction function, HandlerData data);
		void				setFlags(int flags);




	private:
		HandlerFunction		m_function;
		HandlerData			m_data;
		int					m_flags;

};

#endif
