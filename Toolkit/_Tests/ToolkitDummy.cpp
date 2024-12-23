/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolkitDummy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:02:26 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:02:26 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "ToolkitDummy.hpp"

// C++ headers
# include <iostream>

ToolkitDummy::ToolkitDummy() 
    : value(0), m_data(new int[4]), _name("i am a string so long that deffinitely allocates memory on the heap myself") 
{
    // Constructor
}

ToolkitDummy::ToolkitDummy(int value) 
    : value(0), m_data(new int[4]), _name("i am a string so long that deffinitely allocates memory on the heap myself") 
{
    this->value = value;
}

ToolkitDummy::~ToolkitDummy() 
{
    if (m_data) 
        delete[] m_data;
    m_data = NULL;
}

ToolkitDummy::ToolkitDummy(const ToolkitDummy& other) 
    : value(other.value), m_data(new int[4]), _name(other._name) 
{
    std::memcpy(m_data, other.m_data, 4 * sizeof(int));
}

ToolkitDummy& ToolkitDummy::operator=(const ToolkitDummy& other) 
{
    if (!m_data) 
        m_data = new int[4];

    value = other.value;
    _name = other._name;
    std::memcpy(m_data, other.m_data, 4 * sizeof(int));
    return *this;
}

bool ToolkitDummy::operator==(const ToolkitDummy& other) 
{
    return (value == other.value && _name == other._name);
}

bool ToolkitDummy::operator!=(const ToolkitDummy& other) 
{
    return !(value == other.value && _name == other._name);
}

const char* ToolkitDummy::print() const
{
    return "ToolkitDummy: hey there ";
}

std::ostream& operator<<(std::ostream& os, const ToolkitDummy& dummy) 
{
    os << dummy.print() << std::endl;
    return (os);
}