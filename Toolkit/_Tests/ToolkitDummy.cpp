
// Project headers
#include "ToolkitDummy.hpp"

// C++ headers
# include <iostream>

ToolkitDummy::ToolkitDummy() 
    : m_value(0), m_data(new int[4]), m_name("i am a string so long that deffinitely allocates memory on the heap myself") 
{
    // Constructor
}

ToolkitDummy::ToolkitDummy(int value) 
    : m_value(value), m_data(new int[4]), m_name("i am a string so long that deffinitely allocates memory on the heap myself") 
{
}

ToolkitDummy::~ToolkitDummy() 
{
    if (m_data)
        delete[] m_data;
    m_data = NULL;
}

ToolkitDummy::ToolkitDummy(const ToolkitDummy& other) 
    : m_value(other.m_value), m_data(new int[4]), m_name(other.m_name) 
{
    std::memcpy(m_data, other.m_data, 4 * sizeof(int));
}

ToolkitDummy& ToolkitDummy::operator=(const ToolkitDummy& other) 
{
    if (!m_data) 
        m_data = new int[4];

    m_value = other.m_value;
    m_name = other.m_name;
    std::memcpy(m_data, other.m_data, 4 * sizeof(int));
    return *this;
}

bool ToolkitDummy::operator==(const ToolkitDummy& other) 
{
    return (m_value == other.m_value && m_name == other.m_name);
}

bool ToolkitDummy::operator!=(const ToolkitDummy& other) 
{
    return !(m_value == other.m_value && m_name == other.m_name);
}

int ToolkitDummy::getValue() const
{
    return m_value;
}

const std::string& ToolkitDummy::getName() const
{
    return m_name;
}

const char* ToolkitDummy::print() const
{
    return "ToolkitDummy: hey there ";
}



std::ostream& operator<<(std::ostream& os, const ToolkitDummy& dummy) 
{
    os << dummy.getName() << ": " << dummy.getValue() << std::endl;
    return (os);
}