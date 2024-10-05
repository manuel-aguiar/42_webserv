/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pythonCgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:44:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/05 13:10:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pythonCgi.hpp"

PythonCgi::PythonCgi()
{
    
}


PythonCgi::~PythonCgi()
{
    _smallPool->destroy();
    _bigPool->destroy();
}

void   PythonCgi::init()
{

}

void   PythonCgi::reset()
{
    _bigPool->reset();
}

PythonCgi::PythonCgi(const PythonCgi &other)
{
    *this = other;
}

PythonCgi &PythonCgi::operator=(const PythonCgi &other)
{
    if (this == &other)
        return (*this);
    return (*this);
}