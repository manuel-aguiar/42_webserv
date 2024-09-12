/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 13:01:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 13:34:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftcpp.hpp"

#include <iostream>

class Test
{
    public:
        Test(int value) : _value(value) {}
        ~Test() { std::cout << "Test " << _value << "destroyed" << std::endl; }
        int getValue() const { return (_value); }


        static SharedPtr<Test> _createTest(int value)
        {
            return ( SharedPtr<Test>(new Test(value)));
        }


        //implicit conversions, constructor is not <explicit>
        static Test*        _createTestPtr(int value)
        {
            return (new Test(value));
        }
    private:
        int _value;
};

class Testmember
{
    public:
        Testmember(Test* value) : _test(value) {}
        Testmember(const Testmember& copy) : _test(copy._test) {}
        ~Testmember() { std::cout << "Testmember destroyed" << std::endl; }

        const SharedPtr<Test>& getTest() const { return (_test); }
    private:
        SharedPtr<Test> _test;
};

int main(void)
{
    SharedPtr<Test> ptr = Test::_createTest(42);

    SharedPtr<Test> ptr2 = ptr;

    ptr = ptr2;

    

    Testmember tester1(ptr.release());

    Testmember tester2(tester1);

    const SharedPtr<Test>& ptr3 = tester2.getTest();
    //*ptr3 = 23; // forbidden

    return (0);
}