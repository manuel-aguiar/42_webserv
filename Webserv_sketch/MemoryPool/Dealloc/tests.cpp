/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:14:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 09:50:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SharedMemoryPoolDealloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>

int main(void)
{
    
    
    MemoryPool<int> CustomAllocator(4096, 1, 3);
    std::list<int, MemoryPool<int> > list(CustomAllocator);
    
    list.push_back(123);
    list.push_back(345);

    return (0);
}