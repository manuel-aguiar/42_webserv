/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clock.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:22 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 14:52:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLOCK_HPP

# define CLOCK_HPP

# include <ctime>

class Clock
{
    public:
        Clock();
        ~Clock();
        Clock(const Clock& copy);
        Clock& operator=(const Clock& assign);



        void    update();

    private:
        int     _time;
};


#endif