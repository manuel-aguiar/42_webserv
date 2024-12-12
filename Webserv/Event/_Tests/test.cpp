/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 10:40:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/12 10:45:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../Event.hpp"
# include "TestDependencies.hpp"
# include <iostream>

void	EventHandler(Event& event)
{
	(void)event;
	std::cout << "Hello Event" << std::endl;
}

int main(void)
{
	/******************* TEST 1 ************************/
	std::cout << "Test 1: ";
	try
	{
		Event event;

		event.setFd_Data_Handler_Flags(123, &event, EventHandler, 123);

		event.handle();

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (0);
}

