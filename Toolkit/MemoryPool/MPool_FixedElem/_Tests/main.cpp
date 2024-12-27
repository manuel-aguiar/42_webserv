/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 09:49:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MPool_FixedElem.hpp"
#include "../../../List/List.hpp"
#include <list>

int main(void)
{
    std::list<int, MPool_FixedElem<int> > list1(MPool_FixedElem<int>(10));

    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);   
    list1.push_back(3); 
    list1.pop_front();
    list1.pop_front();
    list1.pop_front();
    list1.pop_front();
    list1.push_back(3); 
    list1.pop_front();
    list1.push_back(3);   
    list1.push_back(3);
    list1.pop_front();
    list1.pop_front();
    list1.pop_front();
    list1.push_back(3);   
    list1.push_back(3);

    List<int, MPool_FixedElem<int> > list2(MPool_FixedElem<int>(10));

    list2.push_back(1);
    list2.push_back(2);
    list2.push_back(3);
    list2.push_back(1);
    list2.push_back(2);
    list2.push_back(3);
    list2.push_back(1);
    list2.push_back(2);
    list2.push_back(3);   
    list2.push_back(3); 
    list2.pop_front();
    list2.pop_front();
    list2.pop_front();
    list2.pop_front();
    list2.push_back(3); 
    list2.pop_front();
    list2.push_back(3);   
    list2.push_back(3);
    list2.pop_front();
    list2.pop_front();
    list2.pop_front();
    list2.push_back(3);   
    list2.push_back(3);

}