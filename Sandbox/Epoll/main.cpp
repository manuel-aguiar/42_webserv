/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:32:00 by manuel            #+#    #+#             */
/*   Updated: 2024/08/30 10:46:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/epoll.h>

/*
epoll (epoll_create,
epoll_ctl, epoll_wait), 
*/


/*
    Can use epoll to monitor when CGI script is writing to the write end of the pipe
    and the pipe becomes available for reading :0

    So the server doesn't block while waiting for the CGI to run :0
*/

int main(int ac, char **av)
{

    return (0);
}
