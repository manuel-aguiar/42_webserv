/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpEventHandlers.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:30:33 by manuel            #+#    #+#             */
/*   Updated: 2024/11/08 10:33:21 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPEVENTHANDLERS_HPP

# define HTTPEVENTSHANDLERS_HPP


class Event;
class HttpEventHandlers
{
	static void	httpRead(Event& event);
	static void	httpWrite(Event& event);

};


#endif
