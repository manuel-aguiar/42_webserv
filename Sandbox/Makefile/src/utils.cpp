/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 12:05:36 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/21 12:05:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.hpp"
#include "../include/common.h"
#include <iostream>

void print_utility_message() {
    std::cout << "Utility function called." << std::endl;
    std::cout << "Common constant value: " << COMMON_CONSTANT << std::endl;
}