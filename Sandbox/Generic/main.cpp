/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 16:19:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 16:59:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class IBaseBase
{
    public:
        virtual ~IBaseBase() {}
        IBaseBase() : variable(24) {}
        virtual void print() const = 0; 
        int variable;
};

template <typename T>
class IBase : public IBaseBase
{
    public:
        virtual ~IBase() {}
        virtual void print() const = 0; 
        virtual T& return_value() = 0;
}; 


template <typename T>
class Derived : public IBase<T>
{
    public:
        Derived(T value) : _value(value) {}
        virtual ~Derived() {}
        void print() const { std::cout << _value << std::endl; }
        T& return_value() { return (_value); }

    private:
        T _value;
};

int main(void)
{
    IBaseBase* base = new Derived<int>(42);
    std::cout << base->variable << std::endl;

    IBase<int>* derived = dynamic_cast<IBase<int>*>(base);
    std::cout << derived->variable << std::endl;
    
    base->print();
    delete base;

    return (0);
}