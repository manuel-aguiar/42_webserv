/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:07:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 12:01:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Project headers
#include "../ParameterException.hpp"

//C++ headers
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
