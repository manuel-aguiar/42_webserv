/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:30:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 08:52:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

    Fixed sized blocks. if the alloc is large, ask directly from the system using malloc
    Pool saves a pointer to the current empty block pool
    saves a list of

*/

# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>
# include "../Nginx_MemoryPool.hpp"
# include <cstring>


class ClassThatLivesInPool
{
    public:

        static ClassThatLivesInPool* create(std::string name, size_t age)
        {
            ClassThatLivesInPool*           user;   
            Nginx_MemoryPool*   pool;
            pool = Nginx_MemoryPool::create(1024, 5);
            user = (ClassThatLivesInPool*)pool->allocate(sizeof(ClassThatLivesInPool), true);
            new (user) ClassThatLivesInPool(name, age);
            user->m_pool = pool;
            return (user);
        }


        void destroy()
        {
            this->~ClassThatLivesInPool();
            m_pool->destroy();
        }



        ClassThatLivesInPool() : _name (""), _age(0)
        {

        }
        ClassThatLivesInPool(std::string name, size_t age) : _name(name), _age(age)
        {
            
        }
        std::string         _name;
        size_t              _age;
        
    private:
        Nginx_MemoryPool*   m_pool;
        ~ClassThatLivesInPool() {}
};

class ClassThatHoldsAPool
{
    public:

        ClassThatHoldsAPool() : _name (""), _age(0), m_pool(*Nginx_MemoryPool::create(1024, 5))
        {

        }

        ~ClassThatHoldsAPool()
        {
            m_pool.destroy();
        }


        ClassThatHoldsAPool(std::string name, size_t age) : _name(name), _age(age), m_pool(*Nginx_MemoryPool::create(1024, 5))
        {
        }
            
        std::string         _name;
        size_t              _age;
        
    private:
        Nginx_MemoryPool&   m_pool;
        
};

/*
int main()
{ 
    Nginx_MemoryPool* pool = Nginx_MemoryPool::create(1024, 1);

    std::string& aString = *(std::string *)pool->allocate((sizeof(std::string)), true);
    new (&aString) std::string("bananas"); // placement new, construct the string on the location given
    std::cout << aString << std::endl;

    pool->reset(1);

    std::string& aString2 = *(std::string *)pool->allocate((sizeof(std::string)), true);
    new (&aString2) std::string("bananas"); // placement new, construct the string on the location given
    std::cout << aString2 << std::endl;

    pool->destroy();
}
*/

int main()
{ 
    Nginx_MemoryPool* pool = Nginx_MemoryPool::create(1024, 1);

    for (int i = 0; i < 100; i++)
    {
        std::string& aString = *(std::string *)pool->allocate((sizeof(std::string)), true);
        new (&aString) std::string("bananas"); // placement new, construct the string on the location given
        std::cout << aString << std::endl;
    }
    
    pool->reset(1);
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block


    char *str = (char *)pool->allocate(sizeof("Hello World!"), false);
    std::memcpy(str, "Hello World!", sizeof("Hello World!"));
    
    
    std::cout << str << std::endl;

    pool->destroy();
    
    ClassThatLivesInPool* user = ClassThatLivesInPool::create("Miguel", 25);
    std::cout << "Pool User, name: " << user->_name << ", age: " << user->_age << std::endl;
    user->destroy();

    ClassThatHoldsAPool user2("Antonio", 23);

    std::cout << "Pool Holder, name: " << user2._name << ", age: " << user2._age << std::endl;
    
    return 0;
}

