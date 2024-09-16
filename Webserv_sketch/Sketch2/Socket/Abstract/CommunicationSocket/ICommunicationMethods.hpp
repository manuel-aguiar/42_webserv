/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICommunicationMethods.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 13:49:28 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 13:50:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOMMUNICATIONMETHODS_HPP

# define ICOMMUNICATIONMETHODS_HPP

class ICommunicationMethods
{
    public:
        virtual ~ICommunicationMethods() {}

        virtual void send() = 0;
        virtual void receive() = 0;

};

#endif