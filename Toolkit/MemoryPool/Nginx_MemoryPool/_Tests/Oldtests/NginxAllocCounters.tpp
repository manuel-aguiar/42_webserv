/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NginxAllocCounters.tpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 16:33:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef NGINXALLOCCOUNTERS_TPP

# define NGINXALLOCCOUNTERS_TPP

# include "../Nginx_MemoryPool.hpp"
# include "../Nginx_PoolAllocator.hpp"

class NginxAllocCounter  : public Nginx_MemoryPool
{
	public:
	
		static NginxAllocCounter* create(size_t blockSize, size_t* placeCounters, size_t* placeTotalBytes, size_t startingBlocks = 1)
		{
			// First, allocate raw memory for NginxAllocCounter
			Nginx_MemoryPool* pool = Nginx_MemoryPool::create(blockSize, startingBlocks);
			
			// Use placement new to construct the NginxAllocCounter object in the allocated memory
			NginxAllocCounter* counter = (NginxAllocCounter*)pool->allocate(sizeof(NginxAllocCounter));
			
			counter->m_pool = pool;
			counter->m_placeCounters = placeCounters;
			counter->m_placeTotalBytes = placeTotalBytes;
			return counter;
		}

		// Override allocate function to count allocations
		void* allocate(size_t size)  {
			void* ptr = m_pool->allocate(size);
			(*m_placeCounters)++;  // Increment allocation count
			(*m_placeTotalBytes) += size;
			return ptr;
		}

		void* allocate(size_t size, size_t alignment)  {
			void* ptr = m_pool->allocate(size, alignment);
			(*m_placeCounters)++;  // Increment allocation count
			(*m_placeTotalBytes) += size;
			return ptr;
		}

		// Reset the counter or memory pool as needed
		void reset(int maxBlocks = INT_MAX)  {
			m_pool->reset(maxBlocks);  // Call base class reset
		}

		// Destroy the pool
		void destroy()  {
			m_pool->destroy();  // Call base class destroy
			// Additional cleanup if needed
		}

		Nginx_MemoryPool*	m_pool;
		size_t* 			m_placeCounters;
		size_t* 			m_placeTotalBytes;

};

template <typename T>
class NginxPoolAllocatorCounter : public Nginx_PoolAllocator<T>
{
	public:
		enum counterType
		{
			ALLOC = 0,
			DEALLOC,
			COUNT,
		};

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
			typedef NginxPoolAllocatorCounter<U> other;
		};
		NginxPoolAllocatorCounter(Nginx_MemoryPool* pool, size_t* placeCounters, size_t* totalBytes) : 
			Nginx_PoolAllocator<T>(pool),
			m_placeCounters(placeCounters),
			m_placeTotalBytes(totalBytes)
			{/*std::cout << "allcoator inituialzed: " << pool << std::endl;*/}
			
		NginxPoolAllocatorCounter(const NginxPoolAllocatorCounter& copy) : 
			Nginx_PoolAllocator<T>(copy),
			m_placeCounters(copy.m_placeCounters),
			m_placeTotalBytes(copy.m_placeTotalBytes)
			{(void)copy;}

		~NginxPoolAllocatorCounter() {};

		template <typename U>
		NginxPoolAllocatorCounter(const NginxPoolAllocatorCounter<U>& other) : 
				Nginx_PoolAllocator<T>(other), 
				m_placeCounters(other.getAllocCounter()),
				m_placeTotalBytes(other.getTotalBytes())
			{(void)other;}

		pointer allocate(size_type n, const void* hint = 0)
		{
			(void)hint;
			m_placeCounters[ALLOC]++;
			(*m_placeTotalBytes) += n * sizeof(T);
			return (Nginx_PoolAllocator<T>::allocate(n * sizeof(T)));
		}

		size_t* getAllocCounter() const { return m_placeCounters; }
		size_t* getTotalBytes() const { return m_placeTotalBytes; }

		void deallocate(pointer p, size_type n)
		{
			m_placeCounters[DEALLOC]++;
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

		bool operator==(const NginxPoolAllocatorCounter& other) const { return (Nginx_PoolAllocator<T>::operator==(other)); }
		bool operator!=(const NginxPoolAllocatorCounter& other) const { return (Nginx_PoolAllocator<T>::operator!=(other)); }

		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

		size_t* m_placeCounters;
		size_t* m_placeTotalBytes;
};

#endif