/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolkitBase.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:07:30 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 09:15:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ToolkitBase.hpp"

ToolkitBase::ToolkitBase() 
    : m_data(new int[4]), value(0), _name("Base i am a string so long that deffinitely allocates memory on the heap myself") 
{
}

ToolkitBase::ToolkitBase(int value) 
    : m_data(new int[4]), value(value), _name("Base i am a string so long that deffinitely allocates memory on the heap myself") 
{
}

ToolkitBase::ToolkitBase(const ToolkitBase& other) 
    : m_data(new int[4]), value(other.value), _name(other._name) 
{
    std::memcpy(m_data, other.m_data, 4 * sizeof(int));
}

ToolkitBase& ToolkitBase::operator=(const ToolkitBase& other) 
{
    if (this != &other) {
        if (m_data) {
            delete[] m_data; // Clean up existing data
        }
        m_data = new int[4];
        std::memcpy(m_data, other.m_data, 4 * sizeof(int));
        value = other.value;
        _name = other._name;
    }
    return *this;
}

ToolkitBase::~ToolkitBase() 
{
    if (m_data) {
        delete[] m_data;
        m_data = NULL;
    }
}

bool ToolkitBase::operator==(const ToolkitBase& other) const
{
    return (value == other.value && _name == other._name);
}

bool ToolkitBase::operator!=(const ToolkitBase& other) const
{
    return !(value == other.value && _name == other._name);
}

void ToolkitBase::print() 
{
    std::cout << "Base: " << value << std::endl;
}