/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:07:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/11 16:55:06 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ParameterException.hpp"

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
