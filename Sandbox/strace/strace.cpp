/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strace.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 08:00:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/02 08:13:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char str[] = "malloc starts";

    write(1, str, sizeof(str));

    int *cenas = (int *)malloc(5);

    write(1, str, sizeof(str));
    free(cenas);
    (void)cenas;
    return (0);
}