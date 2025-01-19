

#ifndef TOOLKITDERIVED_HPP

# define TOOLKITDERIVED_HPP

#include "ToolkitBase.hpp"

class ToolkitDerived : public ToolkitBase
{
    public:
        ToolkitDerived();
        ToolkitDerived(int value);
        ToolkitDerived(const ToolkitDerived& other);
        ToolkitDerived& operator=(const ToolkitDerived& other);
        ~ToolkitDerived();

        int getValue() const;

        bool operator==(const ToolkitDerived& other);
        bool operator!=(const ToolkitDerived& other);

        void print();
};


#endif