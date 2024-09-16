/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOnEvents.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:55:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 14:56:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IONEVENTS_HPP

# define IONEVENTS_HPP

class IOnEvents
{
    public:
        virtual ~IOnEvents() {}

        virtual void onClose() = 0;
        virtual void onRead() = 0;
        virtual void onWrite() = 0;
        virtual void onError() = 0;
};


#endif