/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AThread.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:22:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 15:42:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATHREAD_HPP

# define ATHREAD_HPP

# include <iostream>
# include <pthread.h>

/*
    ThreadFunction must be static to allow being called by all members.
    ThreadFunction recasts to a pointer of type AThread, but it is not this
*/

class AThread
{
    public:
        AThread();
        virtual ~AThread();
        void    start();
        void    join();
    
    protected:
        virtual void run() = 0;

    private:

        static void*   ThreadFunction(void* args);

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