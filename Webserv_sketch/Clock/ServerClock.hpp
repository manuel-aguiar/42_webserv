/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 15:52:49 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCLOCK_HPP

# define SERVERCLOCK_HPP

class ServerClock
{
    public:
        ServerClock();
        ~ServerClock();

    private:
        int     _time;
};


#endif