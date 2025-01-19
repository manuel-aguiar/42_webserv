
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