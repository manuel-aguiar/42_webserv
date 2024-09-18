/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poly.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 07:43:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/18 07:59:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>

class Base
{
public:
    virtual ~Base() {}
    virtual void f() = 0;
};

class Derived1 : public Base
{
public:
    void f() {}
};

class Derived2 : public Base
{
public:
    void f() {}
};

int main(void)
{
    Base *b1 = new Derived1();
    Base *b2 = new Derived2();

    for (int i = 0; i < 1000000; i++)
    {
        b1->f();
        b2->f();
    }

    delete b1;
    delete b2;

    return 0;
}