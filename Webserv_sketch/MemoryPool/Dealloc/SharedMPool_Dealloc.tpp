/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMPool_Dealloc.tpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:45:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 10:30:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MemoryPool_Dealloc.hpp"

template <typename T, size_t BlockSize, size_t StartingBLocks, size_t SpareBlocks>
class SharedMPool_Dealloc
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
			typedef SharedMPool_Dealloc<U> other;
		};

		SharedMPool_Dealloc(MemoryPool_Dealloc<T>* ptr = NULL) : 
			_pool(ptr ? ptr : new MemoryPool_Dealloc<T>()), 
			_refCount(new int(1))
		{
		}

		SharedMPool_Dealloc(size_t block_size, size_t starting_blocks, size_t spare_blocks) throw()
		{
			_pool = new MemoryPool_Dealloc<T>(block_size, starting_blocks, spare_blocks);
			_refCount = new int(1);
		}

		SharedMPool_Dealloc(const SharedMPool_Dealloc& other) : _pool(other._pool), _refCount(other._refCount)
		{
			++(*_refCount); 
		}
		
		SharedMPool_Dealloc& operator=(const SharedMPool_Dealloc& other)
		{
			if (this != &other)
			{
				_decrementRefCount();
				_pool = other._pool;
				_refCount = other._refCount;
				++(*_refCount);
			}
			return (*this);
		}
		template <class U> SharedMPool_Dealloc(const SharedMPool_Dealloc<U>& memoryPool) throw()
		{
			_pool = reinterpret_cast<MemoryPool_Dealloc<T>*>(memoryPool._pool);
			_refCount = memoryPool._refCount;
			++(*_refCount);
		}
		~SharedMPool_Dealloc()
		{
			_decrementRefCount();
		}

		pointer address(reference x) const throw() {return _pool->address(x);}
		const_pointer address(const_reference x) const throw() {return _pool->address(x);}

		pointer allocate(size_type n = 1, const_pointer hint = 0){return _pool->allocate(n, hint);}
		void deallocate(pointer p, size_type n = 1) {return _pool->deallocate(p, n);}

		size_type max_size() const throw() {return _pool->max_size();}

		void construct(pointer p, const_reference val) {return _pool->construct(p, val);}
		void destroy(pointer p) {return _pool->destroy(p);}

		pointer newElement(const_reference val) {return _pool->newElement(val);}
		void deleteElement(pointer p) {return _pool->deleteElement(p);}

	//private:
		MemoryPool_Dealloc<T>*      _pool;
		int*                           _refCount;

		void _decrementRefCount()
		{
			if (--(*_refCount) == 0)
			{
				if (_pool)
				{
					delete (_pool);
					_pool = NULL;
				}
				if (_refCount)
				{
					delete _refCount;
					_refCount = NULL;
				}   
			}
		}

		static SharedMPool_Dealloc<T> make_SharedPool()
		{
			return (SharedMPool_Dealloc<T>(new MemoryPool_Dealloc<T>()));
		}

		static SharedMPool_Dealloc<T> make_SharedPool(size_t block_size, size_t starting_blocks, size_t spare_blocks)
		{
			return SharedMPool_Dealloc<T>
				(new MemoryPool_Dealloc<T>(block_size, starting_blocks, spare_blocks)
			);
		}
};

