/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:08:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:01:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../StackArray.hpp"

class ToolkitDummy
{
    public:
        ToolkitDummy() : _someref(*(int *)NULL), _value(0), _arr(new int [4])  {std::cout << " default constructor" << std::endl;};;
        ToolkitDummy(int value) : _someref(*(int *)NULL), _value(value), _arr(new int [4])  {std::cout << "constructor" << std::endl;};
        ~ToolkitDummy()
        {
            if (_arr)
                delete [] _arr;            
            std::cout << "destructor" << std::endl;
        };
        
        ToolkitDummy(const ToolkitDummy& other) : _someref(other._someref), _value(other._value), _arr(new int [4])  {std::cout << "copy" << std::endl;};;
        ToolkitDummy& operator=(const ToolkitDummy& other)
        {
            {std::cout << "assign" << std::endl;};
            if (this == &other)
                return (*this);
            _value = other._value;
            _someref = other._someref;
            if (_arr)
                delete [] _arr;
            _arr = new int [4];
            for (size_t i = 0; i < 4; i++)
            {
                _arr[i] = other._arr[i];
            }
            return (*this);
        }

        int getValue() const
        {
            return (_value);
        }

        void present() const
        {
            std::cout << "yoyo " << _value << std::endl;
        }

        void setValue(int value)
        {
            _value = value;
        }

    private:
        int&    _someref;
        int     _value;
        int*    _arr;
};

std::ostream& operator<<(std::ostream& os, const ToolkitDummy& dummy)
{
    os << "yoyo " << dummy.getValue();
    return (os);
}

int main(void)
{
    StackArray<ToolkitDummy, 10> array1;
    StackArray<ToolkitDummy*, 10> array0;

    for (size_t i = 0; i < array1.size(); i++)
    {
        array0[i] = &array1[i];
        new (&(*array0[i])) ToolkitDummy(i);
    }

    ToolkitDummy cenas = array1[3];
    std::cout << "dummy cenas : " << cenas << std::endl;

    ToolkitDummy tretas = cenas;
    std::cout << "dummy tretas : " << cenas << std::endl;

    ToolkitDummy& ref = tretas;
    std::cout << "dummy ref : " << ref << std::endl;

    ToolkitDummy* ptr = &array1[4];
    std::cout << "dummy ptr : " << *ptr << std::endl;

    StackArray<ToolkitDummy, 10> array3 = array1;
    
    StackArray<ToolkitDummy, 10> array2;
    array2 = array3;

    for (size_t i = 0; i < array2.size(); i++)
    {
        array0[i]->present();
        array2[i].~ToolkitDummy();
    }

    StackArray<int, 10> arr;

    for(size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }

    for(size_t i = 0; i < arr.size(); i++)
    {
        std::cout << arr[i] << std::endl;
    }

    return (0);
}