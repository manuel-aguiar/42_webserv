/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:54:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 18:58:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    c++ -Wall -Wextra -Werror -std=c++98 main1.cpp -o main
*/

#include "../libftcpp.hpp"
# include <iostream>
# include <vector>

int main(void)
{
    //std::vector <UniquePtr<int> > vector;
    //vector.push_back(UniquePtr<int>(new int(42)));

    const UniquePtr<int> ptr(new int(42));

    UniquePtr<int> ptr2 = ptr;  

    return (0);
}