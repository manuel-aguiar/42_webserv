/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolkitDerived.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:10:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 10:58:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ToolkitDerived.hpp"

ToolkitDerived::ToolkitDerived() : ToolkitBase() 
{
    _name = "Derived i am a string so long that deffinitely allocates memory on the heap myself";
}

ToolkitDerived::ToolkitDerived(int value) : ToolkitBase(value) 
{
    _name = "Derived i am a string so long that deffinitely allocates memory on the heap myself";
}

ToolkitDerived::ToolkitDerived(const ToolkitDerived& other) : ToolkitBase(other) 
{
}

ToolkitDerived& ToolkitDerived::operator=(const ToolkitDerived& other) 
{
    if (this != &other) {
        ToolkitBase::operator=(other);
    }
    return *this;
}

ToolkitDerived::~ToolkitDerived() 
{
}

int ToolkitDerived::getValue() const
{
    return value;
}

bool ToolkitDerived::operator==(const ToolkitDerived& other) 
{
    return (value == other.value && _name == other._name);
}

bool ToolkitDerived::operator!=(const ToolkitDerived& other) 
{
    return !(value == other.value && _name == other._name);
}

void ToolkitDerived::print() 
{
    std::cout << "Derived: " << value << std::endl;
}