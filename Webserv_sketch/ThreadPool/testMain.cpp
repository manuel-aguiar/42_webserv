/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 08:33:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/29 17:37:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "include/Concrete/ThreadTask.tpp"
# include "include/Concrete/ThreadPool.hpp"
# include "_legacy/SharedTask.hpp"
# include "_legacy/IndependentTask.hpp"
# include <unistd.h>
/*
    (cd ../.. && ./gitcommit.sh)
    clear && c++ -g -Wall -Wextra -Werror $(find . -name '*.cpp') -lpthread -o indep
*/

pthread_mutex_t globalLock;
void    lockWrite(const std::string& toWrite);

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

unsigned int* allSameThreadUnsafe(unsigned int number)
{
    unsigned int* treta;

    treta = new unsigned int[number];
    return treta;
}


class Test
{
    public:
        static void StaticMethod()
        {
            lockWrite("                 HELLO");
        }

        int ArgYesReturnYes(unsigned int number)
        {
            lockWrite("                 Arg Yes Return Yes " + std::to_string(number));
            return (number);
        };
        int ArgNoReturnYes(void)
        {
            lockWrite("                 Arg No, Return Yes " + std::to_string(52));
            return (42);
        };
        void ArgYesReturnNo(int number)
        {
            lockWrite("                 Arg Yes, Return No " + std::to_string(number));
        };
        void ArgNoReturnNo(void)
        {
            lockWrite("                 Arg No, Return No " + std::to_string(42));
        };


        int ArgYesReturnYes_Const(unsigned int number)
        {
            lockWrite("                 Const: Arg Yes Return Yes " + std::to_string(number));
            return (number);
        };
        int ArgNoReturnYes_Const(void)
        {
            lockWrite("                 Const: Arg No, Return Yes " + std::to_string(52));
            return (42);
        };
        void ArgYesReturnNo_Const(int number)
        {
            lockWrite("                 Const: rg Yes, Return No " + std::to_string(number));
        };
        void ArgNoReturnNo_Const(void) const
        {
            lockWrite("                 Const: Arg No, Return No " + std::to_string(42));
        };         
};



void    lockWrite(const std::string& toWrite)
{
    pthread_mutex_lock(&globalLock);
    std::cout << toWrite << std::endl;
    pthread_mutex_unlock(&globalLock);
}

int main(int ac, char **av)
{
    (void)ac;

    unsigned int count = 50;
    unsigned int vecSize = 10;
    std::vector<long> vector(vecSize);
    Test    dummy;

    unsigned int* save;
    (void)save;
    ThreadPool tp(std::atoi(av[1]));

    const std::string cenas("Hey thhere");

    pthread_mutex_init(&globalLock, NULL);

    for (unsigned int i = 0; i < count; ++i)
    {
        //ThreadTask<long (*)(unsigned int)> task1(fib, i % vecSize);
        //ThreadTask<long (*)(unsigned long)> task2(fibprint, i % vecSize);
        //ThreadTask<void (*)(unsigned long)> task3(voidfibprint, i % vecSize);
        //ThreadTask<void (*)(const std::string&)> task4(printf, cenas);
        //ThreadTask<void (*)()> task5(nada);
        //ThreadTask<void (*)()> task6(StaticMethod::sayHello);
        //ThreadTask<int (Test::*)(int)> task7(dummy, &Test::dosomething, i);
        //tp.addTask(task1);
        //tp.addTask(task2);
        //tp.addTask(task3);
        //tp.addTask(task4);
        //tp.addTask(task5);
        //tp.addTask(task6);
        //tp.addTask(task7);

        //tp.addTask(allSameThreadUnsafe, i, &save);
        
        tp.addTask(fib, i % vecSize);
        tp.addTask(Test::StaticMethod);
        tp.addTask(dummy, &Test::ArgYesReturnYes, i);
        tp.addTask(dummy, &Test::ArgYesReturnNo, (int)i);
        tp.addTask(dummy, &Test::ArgNoReturnYes);
        tp.addTask(dummy, &Test::ArgNoReturnNo);
        tp.addTask(dummy, &Test::ArgYesReturnYes_Const, i);
        tp.addTask(dummy, &Test::ArgYesReturnNo_Const, (int)i);
        tp.addTask(dummy, &Test::ArgNoReturnYes_Const);
        tp.addTask(dummy, &Test::ArgNoReturnNo_Const);
        tp.addTask(fibprint, (unsigned long)(i % vecSize));
        tp.addTask(nada);
        lockWrite("                                           finished inserting tasks");
    }
    
    tp.waitForCompletion();

    pthread_mutex_lock(&globalLock);
    for (unsigned int i = 0; i < vecSize; ++i)
    {
        std::cout << "fib(" << i << ") = " << vector[i] << std::endl;
    }
    std::cout << "tp has " << tp.threadCount() << " threads" << std::endl;
    pthread_mutex_unlock(&globalLock);
    tp.destroy(true);
    pthread_mutex_destroy(&globalLock);


    return (0);
}