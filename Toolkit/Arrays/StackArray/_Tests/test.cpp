/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:08:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:28:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"


// C++ headers
#include <iostream>
#include <cstddef>

int main(void)
{
    StackArray<ToolkitDummy, 10> array1;
    StackArray<ToolkitDummy*, 10> array0;

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

    StackArray<ToolkitDummy, 10> array3 = array1;
    
    StackArray<ToolkitDummy, 10> array2;
    array2 = array3;

    for (size_t i = 0; i < array2.size(); i++)
    {
        array0[i]->print();
        array2[i].~ToolkitDummy();
    }

    StackArray<int, 10> arr;

    for(size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }

    for(size_t i = 0; i < arr.size(); i++)
    {
        std::cout << arr[i] << std::endl;
    }

    return (0);
}