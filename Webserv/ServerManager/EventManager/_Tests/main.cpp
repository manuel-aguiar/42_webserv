/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:50:24 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 14:32:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../EventManager.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../Event/Event.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

// C++ headers
# include <iostream>

extern int TestPart1(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** EventManager tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);

	std::cout << "**************************************************\n" << std::endl;

	return (0);
}

