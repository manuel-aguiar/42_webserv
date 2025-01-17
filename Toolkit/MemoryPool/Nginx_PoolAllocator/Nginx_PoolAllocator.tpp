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

#ifndef NGINX_POOLALLOCATOR_TPP

# define NGINX_POOLALLOCATOR_TPP

# include <limits>

template <typename T, typename MemoryPool>
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
			typedef Nginx_PoolAllocator<U, MemoryPool> other;
		};
<<<<<<< HEAD:Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_PoolAllocator.tpp
		Nginx_PoolAllocator() : m_memoryPool(NULL) {std::cout << "default constructor" << std::endl;}
		Nginx_PoolAllocator(Nginx_MemoryPool* pool) : m_memoryPool(pool) {std::cout << "allcoator inituialzed: " << pool << std::endl;}
		Nginx_PoolAllocator(const Nginx_PoolAllocator& copy) : m_memoryPool(copy.m_memoryPool) {(void)copy;}
		Nginx_PoolAllocator& operator=(const Nginx_PoolAllocator& assign) { m_memoryPool = assign.m_memoryPool;}
=======

		Nginx_PoolAllocator(MemoryPool& pool) : 
			m_memoryPool(&pool) {}

		Nginx_PoolAllocator(const Nginx_PoolAllocator& copy) : 
			m_memoryPool(copy.m_memoryPool) {(void)copy;}

		Nginx_PoolAllocator& operator=(const Nginx_PoolAllocator& copy)
		{
			m_memoryPool = copy.m_memoryPool; 
			return *this;
		}
		~Nginx_PoolAllocator() {};
>>>>>>> main:Toolkit/MemoryPool/Nginx_PoolAllocator/Nginx_PoolAllocator.tpp

		template <typename U>
		Nginx_PoolAllocator(const Nginx_PoolAllocator<U, MemoryPool>& other) : 
			m_memoryPool(&other.getMemPool()) {(void)other;}

		pointer allocate(size_type n, const void* hint = 0)
		{
<<<<<<< HEAD:Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_PoolAllocator.tpp
			assert(m_memoryPool != NULL);
			(void)hint;
			if (n == 0)
				return 0;
			if (n > std::numeric_limits<size_type>::max() / sizeof(T))
				throw std::bad_alloc();
			return static_cast<pointer>(m_memoryPool->allocate(n * sizeof(T), sizeof(T) < sizeof(size_t) ? sizeof(T) : sizeof(size_t)));
=======
			(void)hint;
			
			int 	units;
			size_t 	poolElemSize;

			poolElemSize = m_memoryPool->getElementSize();
			units = (poolElemSize < sizeof(T) ? sizeof(T) / poolElemSize : 1);
			return (reinterpret_cast<pointer>(m_memoryPool->allocate(n * units)));
>>>>>>> main:Toolkit/MemoryPool/Nginx_PoolAllocator/Nginx_PoolAllocator.tpp
		}

		MemoryPool& getMemPool() const {return *m_memoryPool;}

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

		bool operator==(const Nginx_PoolAllocator& other) const { return (m_memoryPool == other.m_memoryPool); }
		bool operator!=(const Nginx_PoolAllocator& other) const { return (m_memoryPool != other.m_memoryPool); }

		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

	private:
		MemoryPool*		m_memoryPool;
		
};




#endif
