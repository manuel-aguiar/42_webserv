/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Interpreter.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:09:30 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/28 10:45:09 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Interpreter.hpp"

HttpManager::HttpManager()
{

}

HttpManager::~HttpManager()
{

}

HttpManager::HttpManager(const HttpManager& copy)
{
    (void)copy;
}

HttpManager& HttpManager::operator=(const HttpManager& assign)
{
    (void)assign;
    return (*this);
}

void    HttpManager::interpret()
{

}
