/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phpCgi.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:45:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 12:46:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phpCgi.hpp"

PhpCgi::PhpCgi()
{
    
}

PhpCgi::~PhpCgi()
{
    
}

PhpCgi::PhpCgi(const PhpCgi &other)
{
    *this = other;
}

PhpCgi &PhpCgi::operator=(const PhpCgi &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}