/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 07:43:41 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/18 08:27:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
class Derived1Func
{
    public:
        static void f() {}
};

class Derived2Func
{
    public:
        static void f() {}
};

class Base
{
    public:
        Base(void (*f)()) : f(f) {}
        void (*f)();
};

typedef Base t_serversocket;
typedef Base t_clientsocket;

#include <vector>

int main(void)
{
    std::vector<Base> v;
    t_serversocket b1(&Derived1Func::f);
    t_clientsocket b2(&Derived2Func::f);
    
    v.push_back(b1);
    v.push_back(b2);

    for (int i = 0; i < 1000000; i++)
    {
        b1.f();
        b2.f();
    }

    return 0;
}