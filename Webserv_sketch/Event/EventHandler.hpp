/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
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
class EventHandler
{
    public:
        static void accept(Event& event);
        static void read(Event& event);
        static void write(Event& event);
        static void Cgi_write(Event& event);
        static void Cgi_read(Event& event);
        static void signalRead(Event& event);
};


#endif