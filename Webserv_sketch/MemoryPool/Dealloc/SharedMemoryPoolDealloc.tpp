/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedMemoryPoolDealloc.tpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:45:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 09:00:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MemoryPoolDealloc.hpp"

template <typename T, size_t BlockSize, size_t StartingBLocks, size_t SpareBlocks>
class SharedMemoryPool
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
			typedef SharedMemoryPool<U> other;
		};

		SharedMemoryPool(MemoryPool<T, BlockSize>* ptr = NULL) : 
			_pool(ptr ? ptr : new MemoryPool<T, BlockSize>()), 
			_refCount(new int(1))
		{
		}

		SharedMemoryPool(const SharedMemoryPool& other) : _pool(other._pool), _refCount(other._refCount)
		{
			++(*_refCount); 
		}
		
		SharedMemoryPool& operator=(const SharedMemoryPool& other)
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
		template <class U> SharedMemoryPool(const SharedMemoryPool<U, BlockSize>& memoryPool) throw()
		{
			_pool = reinterpret_cast<MemoryPool<T, BlockSize>*>(memoryPool._pool);
			_refCount = memoryPool._refCount;
			++(*_refCount);
		}
		~SharedMemoryPool()
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
		MemoryPool<T, BlockSize>*      _pool;
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

		static SharedMemoryPool<T, BlockSize, StartingBLocks, SpareBlocks> make_SharedPool()
		{
			return (SharedMemoryPool<T, BlockSize, StartingBLocks, SpareBlocks>(new MemoryPool<T, BlockSize, StartingBLocks, SpareBlocks>()));
		}

		static SharedMemoryPool<T, BlockSize, StartingBLocks, SpareBlocks> make_SharedPool(size_t block_size, size_t starting_blocks, size_t spare_blocks)
		{
			return SharedMemoryPool<T, BlockSize, StartingBLocks, SpareBlocks>
				(new MemoryPool<T, BlockSize, StartingBLocks, SpareBlocks>(block_size, starting_blocks, spare_blocks)
			);
		}
};

