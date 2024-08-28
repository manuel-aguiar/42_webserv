/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 08:33:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/28 10:22:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "../include/Concrete/ThreadTask.tpp"
# include "../include/Concrete/ThreadPool.hpp"
# include "../include/Concrete/SharedTask.hpp"
# include "../include/Concrete/IndependentTask.hpp"
#include <unistd.h>
/*
    (cd ../.. && ./gitcommit.sh)
    clear && c++ -g -Wall -Wextra -Werror $(find . -name '*.cpp') -lpthread -o indep
*/

long fibtest(unsigned int n)
{
    if (n <= 1)
        return (n);
    return ((fibtest(n - 1) + fibtest(n - 2)));
}


int main(void)
{
    unsigned int count = 10;
    std::vector<long> vector(count);
    ThreadPool tp(10);

    for (unsigned int i = 0; i < count; ++i)
    {
        ThreadTask <long (*)(unsigned int), unsigned int, long> task1(fibtest, i, &vector[i]);
        tp.addTask(task1);
    }
    tp.destroy(true);
    for (unsigned int i = 0; i < count; ++i)
    {
        std::cout << "fib(" << i << ") = " << vector[i] << std::endl;
    }

    return (0);
}

int main5(void)
{
    unsigned int count = 30;
    std::vector<long> vector(count);
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    ThreadPool tp(10);

    for (unsigned int i = 0; i < count; ++i)
    {
        vector[i] = -1;
        SharedTask task1(mutex, i);
        tp.addTask(task1);
    }
    tp.addThread();
    tp.addThread();
    tp.destroy(true);
    pthread_mutex_destroy(&mutex);
    for (unsigned int i = 0; i < count; ++i)
    {
        std::cout << "fib(" << i << ") = " << vector[i] << std::endl;
    }

    return (0);
}


long fib(unsigned long n) {
    if (n <= 1)
        return n;
    return (fib(n - 1) + fib(n - 2));
}

long fibprint(unsigned long n) {
    std::cout << fib(n) << std::endl;
    return (fib(n));
}

void    voidfibprint(unsigned long n)
{
    std::cout << "void " << fib(n) << std::endl;
}

void    printf(const std::string& str)
{
    std::cout << str << std::endl;
}

void nada()
{
    std::cout << "nada" << std::endl;
}

int main123() {
    std::vector<long> results(10);
    
    for (unsigned long i = 0; i < 10; ++i) {
        // Correct instantiation
        ThreadTask<long(*)(unsigned long), unsigned long, long> task1(fib, i, &results[i]);
        ThreadTask<long(*)(unsigned long), unsigned long, long> task2(fibprint, i, NULL);
        ThreadTask<void (*)(unsigned long), unsigned long, void> task3(voidfibprint, i);
        ThreadTask<void (*)(const std::string&), const std::string&, void> task4(printf, "Hey thhere");
        ThreadTask<void(*)(), void, void> task5(nada);
        task1.execute();  // This will store the result of fib(i) in results[i]
        task2.execute();
        task3.execute();
        task4.execute();
        task5.execute();
    }
    
    for (unsigned long i = 0; i < 10; ++i) {
        std::cout << "fib(" << i << ") = " << results[i] << std::endl;
    }

    return 0;
}