/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEventManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 09:25:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/02 09:33:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTMANAGER_HPP

# define IEVENTMANAGER_HPP

class IEventManager
{
    public:
        virtual ~IEventManager() {}
        int add();
};



#endif