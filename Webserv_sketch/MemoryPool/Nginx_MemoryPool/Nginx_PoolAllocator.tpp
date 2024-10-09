/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_PoolAllocator.tpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 10:24:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/07 11:10:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_ALLOCATOR_TPP

# define STRING_ALLOCATOR_TPP

# include "../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
#include <cstdio>
template <typename T>
class Nginx_PoolAllocator
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
			typedef Nginx_PoolAllocator<U> other;
		};

		Nginx_PoolAllocator(Nginx_MemoryPool* pool) : m_memoryPool(pool) {}

		template <typename U>
		Nginx_PoolAllocator(const Nginx_PoolAllocator<U>& other) : m_memoryPool(other.m_memoryPool) {(void)other;}

		pointer allocate(size_type n)
		{
			//std::cout << "allocate called" << std::endl;
			if (n == 0)
				return 0;
			if (n > std::numeric_limits<size_type>::max() / sizeof(T))
				throw std::bad_alloc();
			return static_cast<pointer>(m_memoryPool->allocate(n * sizeof(T), sizeof(T) < sizeof(size_t) ? sizeof(T) : sizeof(size_t)));
		}

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

		bool operator==(const Nginx_PoolAllocator& other) const { return &m_memoryPool == &other.m_memoryPool; }
		bool operator!=(const Nginx_PoolAllocator& other) const { return &m_memoryPool != &other.m_memoryPool; }

		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

	//private:
		Nginx_MemoryPool* m_memoryPool;
};




#endif