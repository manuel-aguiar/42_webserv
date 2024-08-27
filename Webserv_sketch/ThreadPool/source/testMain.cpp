/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 08:33:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 14:54:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "../include/Concrete/ThreadPool.hpp"
# include "../include/Concrete/SharedTask.hpp"
# include "../include/Concrete/IndependentTask.hpp"

/*
    (cd ../.. && ./gitcommit.sh)
    clear && c++ -g -Wall -Wextra -Werror $(find . -name '*.cpp') -lpthread -o indep
*/

int main(void)
{
    int count = 1000;
    std::vector<int> vector(count);
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    ThreadPool tp(10);

    for (int i = 0; i < count; ++i)
    {
        vector[i] = -1;
        SharedTask task(mutex, i);
        tp.addTask(task);
    }
    tp.addThread();
    tp.addThread();
    tp.destroy(true);
    pthread_mutex_destroy(&mutex);
}