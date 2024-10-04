/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:08:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 11:05:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../HeapArray.hpp"

class Dummy
{
    public:
        Dummy() : _someref(*(int *)NULL), _value(0)  {std::cout << " default constructor" << std::endl;};;
        Dummy(int value) : _someref(*(int *)NULL), _value(value) {std::cout << "constructor" << std::endl;};
        ~Dummy() {std::cout << "destructor" << std::endl;};;
        Dummy(const Dummy& other) : _someref(other._someref), _value(other._value) {std::cout << "copy" << std::endl;};;
        Dummy& operator=(const Dummy& other)
        {
            {std::cout << "assign" << std::endl;};
            if (this == &other)
                return (*this);
            _value = other._value;
            _someref = other._someref;
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
};

std::ostream& operator<<(std::ostream& os, const Dummy& dummy)
{
    os << "yoyo " << dummy.getValue();
    return (os);
}

int main(void)
{   
    HeapArray<Dummy*>   array0(10);
    HeapArray<Dummy>    array1(10);

    for (size_t i = 0; i < array1.size(); i++)
    {
        array0[i] = &array1[i];
        new (&(*array0[i])) Dummy(i);
    }

    Dummy cenas = array1[3];
    std::cout << "dummy cenas : " << cenas << std::endl;

    Dummy tretas = cenas;
    std::cout << "dummy tretas : " << cenas << std::endl;

    Dummy& ref = tretas;
    std::cout << "dummy ref : " << ref << std::endl;

    Dummy* ptr = &array1[4];
    std::cout << "dummy ptr : " << *ptr << std::endl;

    HeapArray<Dummy> array3 = array1;
    
    HeapArray<Dummy> array2(10);
    array2 = array3;

    for (size_t i = 0; i < array0.size(); i++)
    {
        array0[i]->present();
    }

    for (size_t i = 0; i < array2.size(); i++)
    {
        array2[i].present();
        array2[i].~Dummy();
    }

    HeapArray<int> arr(10);

    for(size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }



    HeapArray<int> arr2;

    arr2 = arr;

    for(size_t i = 0; i < arr2.size(); i++)
    {
        std::cout << arr2[i] << std::endl;
    }

    return (0);
}