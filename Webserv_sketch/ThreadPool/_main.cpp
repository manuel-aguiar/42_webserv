/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _main.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 08:33:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 12:03:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "ThreadPool.hpp"
# include "SharedTask.hpp"
# include "IndependentTask.hpp"

// clear && c++ -g -Wall -Wextra -Werror  *.cpp -lpthread -o indep

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
    //tp.waitForCompletion();
    //for (unsigned int i = 0; i < vector.size(); ++i)
    //{
    //    std::cout << vector[i] << " ";
    //}
    tp.waitForCompletion();
    pthread_mutex_destroy(&mutex);
    std::cout << std::endl;
}