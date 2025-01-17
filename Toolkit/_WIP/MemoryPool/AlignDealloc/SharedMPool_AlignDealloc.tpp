/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMPool_AlignDealloc.tpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:45:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 08:52:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MemoryPool_AlignDealloc.hpp"

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks >
class SharedMPool_AlignDealloc
{
	public:

		typedef T               value_type;
		typedef T*              pointer;
		typedef T&              reference;
		typedef const T*        const_pointer;
		typedef const T&        const_reference;
		typedef size_t          size_type;
		typedef ptrdiff_t       difference_type;

		template <typename U> struct rebind {
			typedef SharedMPool_AlignDealloc<U> other;
		};

		SharedMPool_AlignDealloc(MemoryPool_AlignDealloc<T>* ptr = NULL) : 
			m_pool(ptr ? ptr : new MemoryPool_AlignDealloc<T>()), 
			m_refCount(new int(1))
		{
		}

		SharedMPool_AlignDealloc(size_t block_size, size_t starting_blocks, size_t spare_blocks) throw()
		{
			m_pool = new MemoryPool_AlignDealloc<T>(block_size, starting_blocks, spare_blocks);
			m_refCount = new int(1);
		}

		SharedMPool_AlignDealloc(const SharedMPool_AlignDealloc& other) : m_pool(other.m_pool), m_refCount(other.m_refCount)
		{
			++(*m_refCount); 
		}
		
		SharedMPool_AlignDealloc& operator=(const SharedMPool_AlignDealloc& other)
		{
			if (this != &other)
			{
				_decrementRefCount();
				m_pool = other.m_pool;
				m_refCount = other.m_refCount;
				++(*m_refCount);
			}
			return (*this);
		}
		template <class U> SharedMPool_AlignDealloc(const SharedMPool_AlignDealloc<U>& memoryPool) throw()
		{
			m_pool = reinterpret_cast<MemoryPool_AlignDealloc<T>*>(memoryPool.m_pool);
			m_refCount = memoryPool.m_refCount;
			++(*m_refCount);
		}
		~SharedMPool_AlignDealloc()
		{
			_decrementRefCount();
		}

		pointer address(reference x) const throw() {return m_pool->address(x);}
		const_pointer address(const_reference x) const throw() {return m_pool->address(x);}

		pointer allocate(size_type n = 1, const_pointer hint = 0){return m_pool->allocate(n, hint);}
		void deallocate(pointer p, size_type n = 1) {return m_pool->deallocate(p, n);}

		size_type max_size() const throw() {return m_pool->max_size();}

		void construct(pointer p, const_reference val) {return m_pool->construct(p, val);}
		void destroy(pointer p) {return m_pool->destroy(p);}

		pointer newElement(const_reference val) {return m_pool->newElement(val);}
		void deleteElement(pointer p) {return m_pool->deleteElement(p);}

	//private:
		MemoryPool_AlignDealloc<T>*		m_pool;
		int*							m_refCount;

		void _decrementRefCount()
		{
			if (--(*m_refCount) == 0)
			{
				if (m_pool)
				{
					delete (m_pool);
					m_pool = NULL;
				}
				if (m_refCount)
				{
					delete m_refCount;
					m_refCount = NULL;
				}   
			}
		}

		static SharedMPool_AlignDealloc<T> make_SharedPool()
		{
			return (SharedMPool_AlignDealloc<T>(new MemoryPool_AlignDealloc<T>()));
		}

		static SharedMPool_AlignDealloc<T> make_SharedPool(size_t block_size, size_t starting_blocks, size_t spare_blocks)
		{
			return SharedMPool_AlignDealloc<T>
				(new MemoryPool_AlignDealloc<T>(block_size, starting_blocks, spare_blocks)
			);
		}
};

