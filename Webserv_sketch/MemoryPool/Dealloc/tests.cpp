/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:14:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 10:02:50 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SharedMPool_Dealloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>

int main(void)
{
    
    
    MemoryPool_Dealloc<int> CustomAllocator(4096, 1, 3);
    std::list<int, MemoryPool_Dealloc<int> > list(CustomAllocator);
    
    list.push_back(123);
    list.push_back(345);

    return (0);
}