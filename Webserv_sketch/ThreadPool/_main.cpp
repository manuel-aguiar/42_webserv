/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _main.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 08:33:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 11:01:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "ThreadPool.hpp"
# include "SharedTask.hpp"
# include "IndependentTask.hpp"

// clear && c++ -Wall -Wextra -Werror  *.cpp -lpthread -o indep

int main(void)
{
    std::vector<int> vector(100);

    ThreadPool tp(5);

    for (int i = 0; i < 100; ++i)
    {
        vector[i] = -1;
        IndependentTask task(vector, i);
        tp.addTask(task);
    }
    tp.waitForCompletion();
    for (unsigned int i = 0; i < vector.size(); ++i)
    {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}