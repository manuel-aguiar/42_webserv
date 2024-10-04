/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:08:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 10:06:34 by mmaria-d         ###   ########.fr       */
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
    HeapArray<Dummy> HeapArray(10);

    for (size_t i = 0; i < HeapArray.size(); i++)
    {
        new (&HeapArray[i]) Dummy(i);
    }

    Dummy cenas = HeapArray[3];
    std::cout << "dummy cenas : " << cenas << std::endl;

    Dummy tretas = cenas;
    std::cout << "dummy tretas : " << cenas << std::endl;

    Dummy& ref = tretas;
    std::cout << "dummy ref : " << ref << std::endl;

    Dummy* ptr = &HeapArray[4];
    std::cout << "dummy ptr : " << *ptr << std::endl;

    for (size_t i = 0; i < HeapArray.size(); i++)
    {
        HeapArray[i]->present();
        HeapArray[i]->~Dummy();
    }

    return (0);
}