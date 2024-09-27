/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:30:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 12:12:26 by mmaria-d         ###   ########.fr       */
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
# include "Nginx_MemoryPool.hpp"
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
            user->_pool = pool;
            return (user);
        }


        void destroy()
        {
            this->~ClassThatLivesInPool();
            _pool->destroy();
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
        Nginx_MemoryPool*   _pool;
        ~ClassThatLivesInPool() {}
};

class ClassThatHoldsaPool
{
    public:

        ClassThatHoldsaPool() : _name (""), _age(0), _pool(*Nginx_MemoryPool::create(1024, 5))
        {

        }

        ~ClassThatHoldsaPool()
        {
            _pool.destroy();
        }


        ClassThatHoldsaPool(std::string name, size_t age) : _name(name), _age(age), _pool(*Nginx_MemoryPool::create(1024, 5))
        {
        }
            
        std::string         _name;
        size_t              _age;
        
    private:
        Nginx_MemoryPool&   _pool;
        
};


int main()
{ 
    Nginx_MemoryPool* pool = Nginx_MemoryPool::create(1024, 10);

    for (int i = 0; i < 100; i++)
    {
        std::string& aString = *(std::string *)pool->allocate((sizeof(std::string)), true);
        new (&aString) std::string("bananas"); // placement new, construct the string on the location given
        std::cout << aString << std::endl;
    }
    
    pool->reset(7);
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block
    pool->allocate(10000, true); // big block


    char *str = (char *)pool->allocate(sizeof("Hello World!"), true);
    std::memcpy(str, "Hello World!", sizeof("Hello World!"));
    
    
    std::cout << str << std::endl;

    pool->destroy();
    
    ClassThatLivesInPool* user = ClassThatLivesInPool::create("Miguel", 25);
    std::cout << "Pool User, name: " << user->_name << ", age: " << user->_age << std::endl;
    user->destroy();

    ClassThatHoldsaPool user2("Antonio", 23);

    std::cout << "Pool Holder, name: " << user2._name << ", age: " << user2._age << std::endl;
    
    return 0;
}

