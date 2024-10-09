/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMPool_Alloc.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:45:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 08:53:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MemoryPool_Alloc.tpp"

template <typename T, size_t BlockSize>
class SharedMPool_Alloc
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
			typedef SharedMPool_Alloc<U> other;
		};

		SharedMPool_Alloc(MemoryPool_Alloc<T, BlockSize>* ptr = NULL) : 
			m_pool(ptr ? ptr : new MemoryPool_Alloc<T, BlockSize>()), 
			m_refCount(new int(1))
		{
			
		}

		SharedMPool_Alloc(const SharedMPool_Alloc& other) : m_pool(other.m_pool), m_refCount(other.m_refCount)
		{
			++(*m_refCount); 
		}
		
		SharedMPool_Alloc& operator=(const SharedMPool_Alloc& other)
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
		template <class U> SharedMPool_Alloc(const SharedMPool_Alloc<U, BlockSize>& memoryPool) throw()
		{
			m_pool = reinterpret_cast<MemoryPool_Alloc<T, BlockSize>*>(memoryPool.m_pool);
			m_refCount = memoryPool.m_refCount;
			++(*m_refCount);
		}
		~SharedMPool_Alloc()
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
		MemoryPool_Alloc<T>*      m_pool;
		int*                      m_refCount;

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

		static SharedMPool_Alloc<T> make_SharedPool()
		{
			return (SharedMPool_Alloc<T>(new MemoryPool_Alloc<T>()));
		}
};

