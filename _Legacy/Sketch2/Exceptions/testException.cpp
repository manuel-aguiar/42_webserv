/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:07:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 09:54:50 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../LibftCpp/libftcpp.hpp"
#include "ParameterException.hpp"   

#include <iostream>

int main()
{
    try
    {
        UniquePtr<int[]> ptr = make_UniqueArray<int>(64);
        ptr[1] = 42;
        std::cout << ptr[1] << std::endl;

        
        throw ParameterException("main", "socket", "Invalid parameter");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}