/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainfd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 14:53:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 15:10:30 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "../FileDescriptor/FdCreators.hpp"
#include "../FileDescriptor/FileDescriptor.hpp"
#include "../libftcpp.hpp"


int main(void)
{
    UniquePtr<FileDescriptor> fd1 = FdCreators::open("main2.cpp", O_RDONLY);
    UniquePtr<FileDescriptor> fd2 = FdCreators::open("mainfd.cpp", O_RDONLY);

    char buffer[3024];

    fd1 = fd2;

    int bytesRead = read((*fd1)(), buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';
    std::cout << buffer << std::endl;
    return (0);
}