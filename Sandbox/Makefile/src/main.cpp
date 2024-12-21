/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 12:05:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/21 12:05:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"
#include "../include/math.hpp"
#include <iostream>

int main() {
    std::cout << "Main program starting..." << std::endl;

    print_utility_message();
    int sum = add(3, 5);
    std::cout << "Sum of 3 and 5: " << sum << std::endl;

    return 0;
}