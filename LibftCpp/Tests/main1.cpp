/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 08:14:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 10:07:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libftcpp.hpp"

#include <iostream>

/*
    c++ -Wall -Wextra -Werror -std=c++98 main1.cpp -o main
*/


class Test
{
    public:
        Test() : _value(0) {}
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
        
        Test address()
        {
            return (*_cenas);
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

    UniquePtr<UniqueHolder> holderholder = make_UniquePtr<UniqueHolder>();

    std::cout << holderholder.get() << std::endl;
    
    UniquePtr<Test> ptr6(new Test(3));
    SharedPtr<Test> shr1(ptr6);

    std::cout << "ptr6 address " << ptr6.get() << std::endl;
    std::cout << "shr1 address " << shr1.get() << std::endl;

    SharedPtr<Test> shr2(shr1);
    std::cout << "shr1 address " << shr1.get() << std::endl;
    std::cout << "shr2 address " << shr2.get() << std::endl;


    UniquePtr<Test> ptr7(new Test(4));
    SharedPtr<Test> shr3(ptr7);

    std::cout << "ptr7 address " << ptr7.get() << std::endl;
    std::cout << "shr3 address " << shr3.get() << std::endl;


    UniquePtr<int[]> ptr123(new int[32]);
    ptr123[1] = 42;
    std::cout << ptr123[1] << std::endl;

    UniquePtr<Test[]> ptr8 = make_UniqueArray<Test>(64);
    UniquePtr<Test[]> ptr9(ptr8);
    SharedPtr<Test[]> shr4 = make_SharedArray<Test>(78);

    shr4 = ptr9;
    SharedPtr<Test[]> shr5 = new Test[32];
    shr5 = ptr9;
    
    UniquePtr<int[]> ptr10 = make_UniqueArray<int>(64);

    ptr10[1] = 42;
    
    std::cout << ptr10[1] << std::endl;
    
    return (0);
}