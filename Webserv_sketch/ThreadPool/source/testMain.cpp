/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 08:33:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/02 07:48:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "../include/Concrete/ThreadTask.tpp"
# include "../include/Concrete/ThreadPool.hpp"
# include "../../_legacy/SharedTask.hpp"
# include "../../_legacy/IndependentTask.hpp"
# include <unistd.h>
/*
    (cd ../.. && ./gitcommit.sh)
    clear && c++ -g -Wall -Wextra -Werror $(find . -name '*.cpp') -lpthread -o indep
*/


pthread_mutex_t globalWriteLock;

void    lockWrite(const std::string& toWrite)
{
    pthread_mutex_lock(&globalWriteLock);
    std::cout << toWrite << std::endl;
    pthread_mutex_unlock(&globalWriteLock);
}

long fibBad(unsigned int n) {
    if (n <= 1)
        return n;
    return (fibBad(n - 1) + fibBad(n - 2));
}

long fibGood(unsigned int n)
{
    long a;
    long b;
    long temp;

    if (n <= 1)
        return (n);
    a = 0;
    b = 1;
    while (n > 0)
    {
        temp = b;
        b = a + b;
        a = temp;
        n--;
    }
    return (a);
}

long fib(unsigned int n)
{
    return (fibBad(n));
}

long fibprint(unsigned long n) {
    lockWrite(std::to_string(fib(n)));
    return (fib(n));
}

void    voidfibprint(unsigned long n)
{
    lockWrite("void " + std::to_string(fib(n)));
}

void    printf(const std::string& str)
{
    lockWrite(str);
}

void nada()
{
    lockWrite("nada");
}

class Test
{
    public:

        int dosomething(int number)
        {
            (void)number;
            lockWrite("                 I WAS GIVEN " + std::to_string(number));
            return (number);
        };

};

class StaticMethod
{
    public:
        static void sayHello()
        {
            lockWrite("                 HELLO");
        }
};

int main(void)
{
    unsigned int count = 50;
    unsigned int vecSize = 10;
    std::vector<long> vector(vecSize);
    Test    dummy;
    ThreadPool tp(2);

    const std::string cenas("Hey thhere");

    pthread_mutex_init(&globalWriteLock, NULL);

    for (unsigned int i = 0; i < count; ++i)
    {
        ThreadTask<long (*)(unsigned int)> task1(fib, i % vecSize);
        ThreadTask<long (*)(unsigned long)> task2(fibprint, i % vecSize);
        ThreadTask<void (*)(unsigned long)> task3(voidfibprint, i % vecSize);
        ThreadTask<void (*)(const std::string&)> task4(printf, cenas);
        ThreadTask<void (*)()> task5(nada);
        ThreadTask<void (*)()> task6(StaticMethod::sayHello);
        ThreadTask<int (Test::*)(int)> task7(dummy, &Test::dosomething, i);
        tp.addTask(task1);
        tp.addTask(task2);
        tp.addTask(task3);
        tp.addTask(task4);
        tp.addTask(task5);
        tp.addTask(task6);
        tp.addTask(task7);
    }
    tp.waitForCompletion();

    pthread_mutex_lock(&globalWriteLock);
    for (unsigned int i = 0; i < vecSize; ++i)
    {
        std::cout << "fib(" << i << ") = " << vector[i] << std::endl;
    }
    std::cout << "tp has " << tp.threadCount() << " threads" << std::endl;
    pthread_mutex_unlock(&globalWriteLock);
    tp.destroy(true);
    pthread_mutex_destroy(&globalWriteLock);


    return (0);
}