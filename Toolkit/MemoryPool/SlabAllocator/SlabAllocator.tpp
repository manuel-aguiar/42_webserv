

#ifndef SLABALLOCATOR_TPP
# define SLABALLOCATOR_TPP

# include <limits>
# include <cstddef>

# include "../../Arrays/DynArray/DynArray.hpp"
# include "../SlabAllocator/SlabAllocator.hpp"

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/

template <typename T, typename MemoryPool>
class SlabAllocator
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
			typedef SlabAllocator<U, MemoryPool> other;
		};


		SlabAllocator(MemoryPool& pool) : 
			m_memoryPool(&pool) {}

		SlabAllocator(const SlabAllocator& copy) : 
			m_memoryPool(copy.m_memoryPool) {(void)copy;}

		SlabAllocator& operator=(const SlabAllocator& copy)
		{
			m_memoryPool = copy.m_memoryPool; 
			return *this;
		}
		~SlabAllocator() {};

		template <typename U>
		SlabAllocator(const SlabAllocator<U, MemoryPool>& other) : 
			m_memoryPool(&other.getMemPool()) {(void)other;}

		pointer allocate(size_type n, const void* hint = 0)
		{
			if (!m_memoryPool)
				throw std::runtime_error("SlabAllocator: no memory pool set");
			(void)n;
			(void)hint;
			return (m_memoryPool->template allocate<T>());
		}

		MemoryPool& getMemPool() const {return *m_memoryPool;}

		void deallocate(pointer p, size_type n)
		{
			(void)n;

			m_memoryPool->template deallocate<T>(p);
		}

		void construct(pointer p, const_reference val)
		{
			new (p) value_type (val);
		}

		void destroy(pointer p)
		{
			p->~value_type();
		}

		bool operator==(const SlabAllocator& other) const { return (m_memoryPool == other.m_memoryPool); }
		bool operator!=(const SlabAllocator& other) const { return (m_memoryPool != other.m_memoryPool); }

		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

	private:
		MemoryPool*		m_memoryPool;
		
};



#endif
