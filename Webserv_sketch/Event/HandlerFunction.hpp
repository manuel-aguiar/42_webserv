/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerFunction.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:54:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 11:04:30 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP

# define EVENTHANDLER_HPP

class Event;
class HandlerFunction
{
    public:
        static void listener_Accept(Event& event);
        static void connection_Read(Event& event);
        static void connection_Write(Event& event);
        static void Cgi_Read(Event& event);
        static void Cgi_Write(Event& event);
        static void signal_Read(Event& event);
};


#endif