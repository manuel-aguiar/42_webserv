/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 08:14:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 08:47:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftcpp.hpp"

#include <iostream>

class Test
{
    public:
        Test(int value) : _value(value) {}
        ~Test() { std::cout << "Test destroyed" << std::endl; }
        int getValue() const { return (_value); }


        static UniquePtr<Test> _createTest(int value)
        {
            UniquePtr<Test> unique(new Test(value));
            return ( UniquePtr<Test>)(new Test(value));
        }
    private:
        int _value;
};

int main()
{
    UniquePtr<Test> ptr = Test::_createTest(42);
    std::cout << "ptr value: " << ptr->getValue() << std::endl;

    UniquePtr<Test> ptr2(Test::_createTest(21));
    std::cout << "ptr value: " << ptr2->getValue() << std::endl;

    return (0);
}