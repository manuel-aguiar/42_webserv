/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:30:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 08:35:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP

# define SOCKET_HPP

# include "sys/socket.h"

# include "../FileDescriptor/FileDescriptor.hpp"

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

#endif