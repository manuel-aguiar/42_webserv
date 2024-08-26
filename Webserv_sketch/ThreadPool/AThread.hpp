/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AThread.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:22:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 15:17:29 by mmaria-d         ###   ########.fr       */
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
        void    startAndRun();
        void    join();
    
    private:

        void*   MyThreadFunction(void* args);

        enum EThreadState
        {
            EThread_Unitialized,
            EThread_Initialized,
            EThread_Joined
        };

        EThreadState    _state;
        pthread_t       _thread;

        AThread(const AThread& copy);
        AThread& operator=(const AThread& assign);
};



#endif