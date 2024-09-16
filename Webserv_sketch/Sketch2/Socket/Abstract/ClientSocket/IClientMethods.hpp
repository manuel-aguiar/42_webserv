/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IClientMethods.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 13:48:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 13:52:30 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICLIENTMETHODS_HPP

# define ICLIENTMETHODS_HPP

class IClientMethods
{
    public:
        virtual ~IClientMethods() {}

        virtual void connect() = 0;
        virtual void disconnect() = 0;

};

#endif