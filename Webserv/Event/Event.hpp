/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:15 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/26 10:19:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP

# define EVENT_HPP

class Event
{
    public:
        Event();
        ~Event();

        typedef void	(*HandlerFunction)(Event& event);
        typedef void*	HandlerData;

		HandlerData			getData();
		HandlerFunction		getFunction();
		int					getFlags();

        void				setHandlerFunction_and_Data(HandlerFunction function, HandlerData data);
        void				setFlags(int flags);


		
        void				handle();

	private:
        HandlerFunction    m_function;
        HandlerData        m_data;
        int                m_flags;

        Event(const Event& copy);
        Event& operator=(const Event& assign);
};

#endif
