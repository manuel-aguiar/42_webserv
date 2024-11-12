/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEventHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:37:32 by codespace         #+#    #+#             */
/*   Updated: 2024/11/12 10:39:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIEVENTHANDLER_HPP

# define CGIEVENTHANDLER_HPP

class Event;

class CgiEventHandler
{
	public:
		static void cgi_Read(Event& event);
		static void cgi_Write(Event& event);
};



#endif
