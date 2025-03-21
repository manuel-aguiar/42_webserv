
#ifndef TOOLKITBASE_HPP

# define TOOLKITBASE_HPP

#include <string>
#include <cstring>
#include <iostream>

class ToolkitBase
{
    public:
        ToolkitBase();
        ToolkitBase(int value);
        ToolkitBase(const ToolkitBase& other);
        ToolkitBase& operator=(const ToolkitBase& other);
        virtual ~ToolkitBase();

        virtual int getValue() const;

        bool operator==(const ToolkitBase& other);
        bool operator!=(const ToolkitBase& other);

        virtual void print();

    protected:
        int* m_data;
        int value;
        std::string _name;
};

#endif