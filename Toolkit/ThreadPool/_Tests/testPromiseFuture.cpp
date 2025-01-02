/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testPromiseFuture.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 22:40:28 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 15:09:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "../ThreadPool/ThreadPool.hpp"


# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>

# include "../../MemoryPool/MemoryPool.h"
# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../Arrays/StackArray/StackArray.hpp"
# include "../../_Tests/test.h"

static long fibGood(unsigned int n)
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


template<typename T>
struct State
{
    State() :  m_result(0), m_ready(false)
    {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_cond, NULL);
    }
    ~State()
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }
    T       get()
    {
        pthread_mutex_lock(&m_mutex);
        while (!m_ready)
            pthread_cond_wait(&m_cond, &m_mutex);
        pthread_mutex_unlock(&m_mutex);
        return (m_result);
    }

    void    set(const T& value) 
    {
        pthread_mutex_lock(&m_mutex);
        m_result = value;
        m_ready = true;
        pthread_cond_signal(&m_cond);
        pthread_mutex_unlock(&m_mutex);
    }

    pthread_mutex_t m_mutex;
    pthread_cond_t  m_cond;
    T               m_result;
    bool            m_ready;
};

template <typename T>
class Future
{
    public:
        Future() {}
        
        Future(State<T>& m_state) : m_state(m_state) {}
        ~Future() {}
        Future(const Future& other) : m_state(other.m_state) {}
        Future& operator=(const Future& other)
        {
            if (this != &other)
                m_state = other.m_state;
            return (*this);
        }


        long get()
        {
            return (m_state.get());
        }

    private:
        State<T>&    m_state;
};

template <typename T>
class Promise : public IThreadTask
{
    public:

        Promise(size_t n = 0) : m_n(n), m_askedFuture(false), m_state()
        {
            
        }
        
        void execute()
        {
            set_value(fibGood(m_n));
        }
        
        void    set_value(const T& value)
        {
            m_state.set(value);
        }

        Future<T>   get_future()
        {
            if (m_askedFuture)
                throw std::runtime_error("Future already asked");
            m_askedFuture = true;
            return (Future<T>(m_state));
        }

        
    private:
        size_t 	                m_n;
        bool                    m_askedFuture;
        State<T>                m_state;
};

int TestPart3(int testNumber)
{
/************************************************************** */
	std::cout << "TEST " << testNumber << ": ";
	try
	{

		ThreadPool 					    tp(10, 20);
		size_t 		                    fiboExpected;

		Promise<size_t>	                promise(50);
        Future<size_t>                  future = promise.get_future();
        

        fiboExpected = fibGood(50);
        tp.addTask(promise);

        size_t result = future.get();           // code blocks until this value is available

        if (result != fiboExpected)
            throw std::runtime_error("Didn't calculate fibonacci right");

        tp.waitForCompletion();

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
	testNumber++;	
	return (testNumber);
}
