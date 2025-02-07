/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poly.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 07:43:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/02/07 15:55:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>

class ConnInfo
{
public:
    virtual ~ConnInfo() {}
    virtual void f() = 0;
};

class Derived1 : public ConnInfo
{
public:
    void f() {}
};

class Derived2 : public ConnInfo
{
public:
    void f() {}
};

int main(void)
{
    ConnInfo *b1 = new Derived1();
    ConnInfo *b2 = new Derived2();

    for (int i = 0; i < 1000000; i++)
    {
        b1->f();
        b2->f();
    }

    delete b1;
    delete b2;

    return 0;
}