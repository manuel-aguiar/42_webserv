/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AThread.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:22:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 13:30:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATHREAD_HPP

# define ATHREAD_HPP

# include <iostream>
# include <pthread.h>

class AThread
{
    public:
        AThread();
        virtual ~AThread();
    private:
        AThread(const AThread& copy);
        AThread& operator=(const AThread& assign);
};



#endif