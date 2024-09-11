/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:07:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 10:08:30 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParameterException.hpp"   

#include <iostream>

int main()
{
    try
    {
        throw ParameterException("main", "socket", "Invalid parameter");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}