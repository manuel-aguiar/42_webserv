/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 08:14:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/12 19:06:25 by mmaria-d         ###   ########.fr       */
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


        //implicit conversions, constructor is not <explicit>
        static UniquePtr<Test> _createTest(int value)
        {
            Test* cenas = new Test(value);
            return (UniquePtr<Test>(cenas));
        }

    private:
        int _value;

};

class UniqueHolder
{
    public:

        UniqueHolder() : _cenas(NULL) {}

        UniqueHolder(UniquePtr<Test>& cenas) : _cenas(cenas) {}

        UniqueHolder(const UniqueHolder& outro);
        UniqueHolder& operator=(const UniqueHolder& outro);

        UniqueHolder(UniqueHolder& outro) : _cenas(outro._cenas) {}
        
        UniqueHolder& operator=(UniqueHolder& outro)
        {
            if (this == &outro)
                return (*this);
            
            _cenas = outro._cenas;

            return (*this);
        }

        int print()
        {
            return (_cenas->getValue());
        }
    private:
        UniquePtr<Test> _cenas;
};


int main()
{

    /* for unique pointers one must allow construction from non const sources */

    /*
        UniquePtr<Test> ptr = Test::_createTest(42);
        //doesn't compile because of lack move constructor support
    */

    /*
        UniquePtr<Test> ptr;
        ptr = Test::_createTest(42);
        //doesn't compile because of lack move constructor support
    */

    UniquePtr<Test> ptr1(new Test(42));
    UniquePtr<Test> ptr2 = Test::_createTest(37);

    ptr2 = ptr1;
    UniquePtr<Test> ptr3(ptr2);
    
    std::cout << "ptr1 address: " << ptr1.get() << std::endl;
    std::cout << "ptr2 address: " << ptr2.get() << std::endl;
    std::cout << "ptr3 value: " << ptr3->getValue() << std::endl << std::endl;

    UniquePtr<Test> ptr4;

    ptr4 = ptr3;

    std::cout << "ptr3 address: " << ptr3.get() << std::endl;
    std::cout << "ptr4 value: " << ptr4->getValue() << std::endl << std::endl;

    UniquePtr<Test> ptr5 = make_UniquePtr<Test>(32);

    std::cout << "ptr4 address: " << ptr4.get() << std::endl;
    std::cout << "ptr5 value: " << ptr5->getValue() << std::endl << std::endl;

    UniqueHolder holder(ptr5);

    std::cout << holder.print() << std::endl;

    UniqueHolder holder2(holder);

    std::cout << holder2.print() << std::endl;

    UniqueHolder holder3;

    holder3 = holder2; 

    std::cout << holder3.print() << std::endl;

/*
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


    UniquePtr<int> ptr7(new int(4));
    int* rawPtr7 = ptr7.release();  // Manually release the pointer, now ptr1 no longer owns it
    std::cout << "rawPtr = " << *rawPtr7 << std::endl; // rawPtr still points to the resource
    delete rawPtr7;

    Testmember testmember(new Test(42));
    std::cout << "testmember value: " << testmember.getTest().get() << std::endl;
    Testmember testmember2(testmember);
    std::cout << "testmember value: " << testmember.getTest().get() << std::endl;
    std::cout << "testmember2 value: " << testmember2.getTest().get() << std::endl;

    const UniquePtr<Test>& check = testmember2.getTest();
    *check = 37;
    std::cout << "testmember2 value: " << testmember2.getTest().get()->getValue() << std::endl;
    UniquePtr<Test> steal = check;

    std::cout << "testmember2 value: " << testmember2.getTest().get()->getValue() << std::endl;
*/
    return (0);
}