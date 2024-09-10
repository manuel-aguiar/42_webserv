/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:14:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 14:21:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../ServerSocket.hpp"

int main(void)
{
    ServerSocket socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    return (0);
}