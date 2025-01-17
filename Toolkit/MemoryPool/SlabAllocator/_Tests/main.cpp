/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<<< HEAD:Webserv/ServerManager/ServerManagerGetSetAccess.cpp
/*   ServerManagerGetSetAccess.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:03:04 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/03 09:45:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Project headers
# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig.hpp"

//C++ headers
# include <cassert>

//getters
const ServerConfig&	ServerManager::getConfig() const
{
	return (m_config);
}

//accessors
t_ptr_ProtoModule		ServerManager::accessProtoModule(e_protoModules module)
{
	return (m_protoModules[module]);
}
========
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:41:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/13 00:30:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//C++ headers
#include <iostream>

//extern int TestPart1(int testNumber);

int main(void)
{
    //int testNumber = 1;

	std::cout << "\n*************** SlabAllocator tests ***************" << std::endl;
	
    //testNumber = TestPart1(testNumber);

	std::cout << "**********************************************************\n" << std::endl;

    return 0;
}

>>>>>>>> main:Toolkit/MemoryPool/SlabAllocator/_Tests/main.cpp
