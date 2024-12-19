/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:13:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/19 15:29:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../StaticAssert.hpp"

int main()
{
    STATIC_ASSERT(sizeof(int) == 4, test1);     // pass
    //STATIC_ASSERT(sizeof(int) == 8, test2);   // fail, WON'T COMPILE
    return (0);
}