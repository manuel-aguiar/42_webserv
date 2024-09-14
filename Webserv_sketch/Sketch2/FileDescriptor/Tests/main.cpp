/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:55:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/14 11:00:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../FileDescriptor.hpp"

#include <iostream>

#include <vector>

int main()
{
    FileDescriptor fd(1);
    FileDescriptor fd2(2);

    if (fd == 1)
        std::cout << "fd is 1" << std::endl;
    if (fd2 == 2)
        std::cout << "fd2 is 2" << std::endl;
    if (fd < 2)
        std::cout << "fd is less than 2" << std::endl;
    if (fd2 < 3)
        std::cout << "fd2 is less than 3" << std::endl;
    
    std::cout << fd() << std::endl; 

    std::vector<FileDescriptor> fds;

    fds.push_back(fd);

    return (0);
}