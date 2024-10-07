/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:55:08 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/14 12:03:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../FileDescriptor.hpp"

# include "../../../../LibftCpp/libftcpp.hpp"

#include <iostream>

#include <vector>

int main()
{
    UniquePtr<FileDescriptor> fd = make_UniquePtr<FileDescriptor>(2);
    UniquePtr<FileDescriptor> fd2 = make_UniquePtr<FileDescriptor>(1);

    if (fd->getFd() == 1)
        std::cout << "fd is 1" << std::endl;
    if (fd2->getFd() == 2)
        std::cout << "fd2 is 2" << std::endl;
    if (fd->getFd() < 2)
        std::cout << "fd is less than 2" << std::endl;
    if (fd2->getFd() < 3)
        std::cout << "fd2 is less than 3" << std::endl;
    
    std::cout << fd->getFd() << std::endl; 

    std::vector<UniquePtr<FileDescriptor> > fds;

    fds.push_back(fd);
    fds.push_back(fd);
    fds.push_back(fd);
    fds.push_back(fd);
    fds.push_back(fd);
    fds.push_back(fd);
    fds.push_back(fd);
    fds[6]->getFd();

    return (0);
}