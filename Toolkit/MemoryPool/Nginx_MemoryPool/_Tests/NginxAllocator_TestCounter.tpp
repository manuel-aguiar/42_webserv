/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NginxAllocator_TestCounter.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 17:22:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 17:35:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINXALLOCATOR_TESTCOUNTER_HPP

# define NGINXALLOCATOR_TESTCOUNTER_HPP

# include "../Nginx_PoolAllocator.hpp"
# include "NginxMemPool_TestCounter.hpp"

template <typename T>
class NginxAllocator_TestCounter : public Nginx_PoolAllocator<T>
{
    public:

		typedef T					value_type;
		typedef T*					pointer;
		typedef const T*			const_pointer;
		typedef T&					reference;
		typedef const T&			const_reference;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;

        template <typename U>
		struct rebind
		{
			typedef NginxAllocator_TestCounter<U> other;
		};

		NginxAllocator_TestCounter(Nginx_MemoryPool& pool, AllocationStats* placeCounters) : Nginx_PoolAllocator<T>(pool), m_placeCounters(placeCounters) {/*std::cout << "allcoator inituialzed: " << pool << std::endl;*/}
		NginxAllocator_TestCounter(const NginxAllocator_TestCounter& copy) : Nginx_PoolAllocator<T>(copy), m_placeCounters(copy.m_placeCounters) {(void)copy;}

		~NginxAllocator_TestCounter() {};

		template <typename U>
		NginxAllocator_TestCounter(const NginxAllocator_TestCounter<U>& other) : Nginx_PoolAllocator<T>(other), m_placeCounters(other.getStats()) {(void)other;}

		pointer allocate(size_type n, const void* hint = 0)
		{
			(void)hint;
			if (n == 0)
				return 0;
			if (n > max_size())
				throw std::bad_alloc();
            m_placeCounters->m_allocCount++;
            m_placeCounters->m_allocBytes += n * sizeof(T);
			return (Nginx_PoolAllocator<T>::allocate(n, hint));
		}

        AllocationStats* getStats() const { return m_placeCounters; }

		void deallocate(pointer p, size_type n)
		{
			(void)p;
			(void)n;
		}

		void construct(pointer p, const_reference val)
		{
			new (p) value_type (val);
		}

		void destroy(pointer p)
		{
			p->~value_type();
		}

		bool operator==(const NginxAllocator_TestCounter& other) const { return (Nginx_PoolAllocator<T>::operator=(other)); }
		bool operator!=(const NginxAllocator_TestCounter& other) const { return (Nginx_PoolAllocator<T>::operator!=(other)); }

		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}
    private:
        AllocationStats* m_placeCounters;
};


#endif