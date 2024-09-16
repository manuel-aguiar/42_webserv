/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:39:16 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:39:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__socket.h"

int main(void)
{
    IPv4Address                                     ipv4(0, 8080);

    ServerSocket<IPv4Address> server(ipv4, SOCK_STREAM, IPPROTO_TCP);
}