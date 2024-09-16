/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IFunctionsCommunication.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:02:21 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:44:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMUNICATIONFUNCTIONS_HPP

# define ICOMMUNICATIONFUNCTIONS_HPP

class ICommunicationFunctions
{
    public:
        virtual ~ICommunicationFunctions() {}
        virtual void send() = 0;
        virtual void receive() = 0;
};


#endif