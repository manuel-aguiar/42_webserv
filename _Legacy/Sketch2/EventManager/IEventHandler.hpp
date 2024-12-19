/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:01:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/17 11:13:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTHANDLER_HPP

# define IEVENTHANDLER_HPP

class IEventHandler
{
    public:
        virtual ~IEventHandler() {}

        virtual void    onRead() = 0;
        virtual void    onWrite() = 0;
        virtual void    onError() = 0;
        virtual void    onClose() = 0;
};


#endif