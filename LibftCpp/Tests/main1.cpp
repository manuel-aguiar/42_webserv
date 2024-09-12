/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 08:14:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 09:23:42 by mmaria-d         ###   ########.fr       */
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
            return ( UniquePtr<Test>)(new Test(value));
        }

        static Test*        _createTestPtr(int value)
        {
            return (new Test(value));
        }
    private:
        int _value;
};


int main()
{
    UniquePtr<Test> ptr = Test::_createTest(42);
    std::cout << "ptr value: " << ptr->getValue() << std::endl;

    UniquePtr<Test> ptr2(ptr);

    if(ptr2->getValue() != 42)
        std::cout << "not passed" << std::endl;
    if (ptr.get() != NULL)
        std::cout << "not passed" << std::endl;

    UniquePtr<Test> ptr3;

    ptr3 = ptr2;

    if(ptr3->getValue() != 42)
        std::cout << "not passed" << std::endl;
    if (ptr2.get() != NULL)
        std::cout << "not passed" << std::endl;

    std::cout << "ptr3 value: " << ptr3->getValue() << std::endl;

    UniquePtr<Test> ptr4(new Test(123));
    
    std::cout << "ptr4 value: " << ptr4->getValue() << std::endl;

    ptr4 = ptr3;
    std::cout << "ptr4 value: " << ptr4->getValue() << std::endl;


    //implicit conversion of a pointer to a unique pointer
    // constructor is not <explicit>, if that was the case, no implicit promotions are possible
    ptr4 = Test::_createTestPtr(321);
    
    std::cout << "ptr4 value: " << ptr4->getValue() << std::endl;

    UniquePtr<int> ptr11(new int(42));
    UniquePtr<int> ptr12(ptr11);

    std::cout << "ptr11 = " << ptr11.get() << std::endl;
    std::cout << "ptr12 = " << ptr12.get() << std::endl;

    ptr12 = ptr12;

    std::cout << "ptr12 = " << *ptr12 << std::endl;


    UniquePtr<int> ptr6(new int(3));
    int* rawPtr = ptr6.get();
    
    ptr6.reset(rawPtr); // Attempt to reset to the same pointer

    std::cout << "ptr1 value: " << *ptr6 << std::endl;


    return (0);
}