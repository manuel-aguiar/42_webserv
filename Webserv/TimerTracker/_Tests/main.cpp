/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:52:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 19:02:37 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>

extern int TestPart1(int testNumber);

int main(void)
{	
    int testNumber = 1;
    
    std::cout << "\n*************** TimerTracker tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);

    std::cout << "******************************************************\n" << std::endl;
    
    return (0);
}