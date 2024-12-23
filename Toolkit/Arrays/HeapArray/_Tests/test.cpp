/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:08:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:30:33 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "../HeapArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"

// C++ headers
#include <iostream>

int main(void)
{   
    HeapArray<ToolkitDummy*>   array0(10);
    HeapArray<ToolkitDummy>    array1(10);

    HeapArray<int>      array4(10);

    for (size_t i = 0; i < array1.size(); i++)
    {
        array0[i] = &array1[i];
        new (&(*array0[i])) ToolkitDummy(i);
    }

    ToolkitDummy cenas = array1[3];
    std::cout << "dummy cenas : " << cenas << std::endl;

    ToolkitDummy tretas = cenas;
    std::cout << "dummy tretas : " << cenas << std::endl;

    ToolkitDummy& ref = tretas;
    std::cout << "dummy ref : " << ref << std::endl;

    ToolkitDummy* ptr = &array1[4];
    std::cout << "dummy ptr : " << *ptr << std::endl;

    HeapArray<ToolkitDummy> array3 = array1;
    
    HeapArray<ToolkitDummy> array2(10);
    array2 = array3;

    for (size_t i = 0; i < array0.size(); i++)
    {
        array0[i]->print();
    }

    for (size_t i = 0; i < array2.size(); i++)
    {
        array2[i].print();
        array2[i].~ToolkitDummy();
    }

    HeapArray<int> arr(10);

    for(size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }



    HeapArray<int> arr2;

    arr2 = arr;

    for(size_t i = 0; i < arr2.size(); i++)
    {
        std::cout << arr2[i] << std::endl;
    }

    return (0);
}