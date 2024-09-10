/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:52:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 09:01:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

class Socket : public FileDescriptor {
public:
    Socket() : FileDescriptor() {
        // Constructor implementation
    }

    Socket(const Socket& other) : FileDescriptor(other) {
        // Copy constructor implementation
    }

    Socket& operator=(const Socket& other) {
        if (this != &other) {
            FileDescriptor::operator=(other);
            // Copy assignment implementation
        }
        return *this;
    }

    ~Socket() {
        // Destructor implementation
    }
};