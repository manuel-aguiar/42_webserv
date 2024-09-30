/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:12:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 11:15:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventManager.hpp"

EventManager::EventManager()
{

}

EventManager::~EventManager()
{
    
}



//private

EventManager::EventManager(const EventManager& copy)
{
    (void)copy;
}

EventManager& EventManager::operator=(const EventManager& assign)
{
    (void)assign;
    return (*this);
}