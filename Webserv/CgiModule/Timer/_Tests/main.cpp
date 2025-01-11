/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:00:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 12:01:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Timer.hpp"
# include <iostream>

int main(void)
{
    Timer timer = Timer::now();

    std::cout << "Seconds: " << timer.seconds() << std::endl;

    timer += 5000;

    std::cout << "Seconds: " << timer.seconds() << std::endl;
    return (0);
}